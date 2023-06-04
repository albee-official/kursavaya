#define F_CPU 8000000UL				// ����������� �������� ������� (8 ���)
#define RS 2
#define E 6

#define LCD_CLOCK_UPDATE_INTERVAL 1 // in seconds
#define LCD_TIMER_UPDATE_INTERVAL .1 // in seconds

#include "Program.h"
#include "../LCD/LCD.h"
#include "../Buttons/Buttons.h"

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

char clock_template[18] = {'T', 'i', 'm', 'e', ':', ' ', '0', '0', ':', '0', '0', ':', '0', '0', ' ', ' ', ' ', ' '};
char timer_template[18] = {'T', 'i', 'm', 'e', 'r', ':', ' ', '0', '0', ':', '0', '0', ':', '0', '0', '0', ' ', ' '};
	
char bottom_template[18] = {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '};
char changing_time_template[18] = {'S', 'e', 't', ':', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '};

Program::Program() {
	lcd = LCD();
	buttons = Buttons();
	
	state = 0;
}

Program::~Program() {}

////////////////////////////////////////////////////////////////////////

// update calls update the buttons and logic overall
	
void Program::Update() {
	switch (state) {
		case 0: ClockUpdate(); break;
		case 1: TimerUpdate(); break;
	}
}

void Program::ClockUpdate() {
	if (buttons.IsPressed()) {
		if (buttons.IsDown(0)) {
			state = 1;
		}
		
		if (buttons.IsDown(1)) {
			clock_on = !clock_on;
		}
		
		if (buttons.IsDown(2)) {
			change_digit_index = (change_digit_index + 1) % 6;
		}
		
		if (buttons.IsDown(3)) {
			// increase the digit by one
			switch (change_digit_index) {
				case 0: cached_hours = (cached_hours + 10) % 24; break;
				case 1: 
					if ((cached_hours % 10) == 9) cached_hours -= 10;
					cached_hours = (cached_hours + 1) % 24; 
					break;
				
				case 2: cached_minutes = (cached_minutes + 10) % 60; break;
				case 3: 
					if ((cached_minutes % 10) == 9) cached_minutes -= 10;
					cached_minutes = (cached_minutes + 1) % 60; 
					break;
				
				case 4: cached_seconds = (cached_seconds + 10) % 60; break;
				case 5: 
					if ((cached_seconds % 10) == 9) cached_seconds -= 10;
					cached_seconds = (cached_seconds + 1) % 60;	
					break;
			}
			
			clock_time = cached_seconds + cached_minutes * 60 + cached_hours * 3600;
			lcd_update_counter = clock_time;
		}
		
		Draw();
	}
}

void Program::TimerUpdate() {
	if (buttons.IsPressed()) {
		if (buttons.IsDown(0)) {
			state = 0;
		}
		
		if (buttons.IsDown(1)) {
			timer_on = !timer_on;
		}
		
		if (buttons.IsDown(2)) {
			timer = 0;
		}
		
		Draw();
	}
}

// draw calls update the LCD

void Program::Draw() {
	// draw the first row depending on which state we are in
	switch (state) {
		case 0: ClockDraw(); break;
		case 1: TimerDraw(); break;
	}
	
	if (!clock_on) {
		// char changing_time_template[18] = {'S', 'e', 't', ':', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '};
		
		char temp_template[18];
		for (int i = 0; i < 18; i++) {
			temp_template[i] = changing_time_template[i];
		}
		
		char offset = change_digit_index + 6;
		if (change_digit_index > 1) offset++;
		if (change_digit_index > 3) offset++;
		temp_template[offset] = '^';
			
		lcd.SendCommand(LCD::SECOND_ROW);
		lcd.WriteString(temp_template, 18); // write bottom template with state inserted. 16 characters so that it doesn't overflow into other templates
		
	} else {
		// draw the second row
		lcd.SendCommand(LCD::SECOND_ROW);
		lcd.WriteString(bottom_template, 18); // write bottom template with state inserted. 16 characters so that it doesn't overflow into other templates
	}
}

void Program::ClockDraw() {
	// char clock_template[16] = {'T', 'i', 'm', 'e', ':', ' ', '0', '0', ':', '0', '0', ':', '0', '0', ' ', ' '};
		
	if (clock_on) { // update time if we are not changing it
		int temp_time = floor(clock_time);
		cached_seconds = temp_time % 60;
		cached_minutes = (temp_time / 60) % 60;
		cached_hours = temp_time / 3600;
	}
	
	int s = cached_seconds;
	int m = cached_minutes;
	int h = cached_hours;
	
	clock_template[13] = (s % 10) + 48; s /= 10;			// low seconds
	clock_template[12] = (s % 10) + 48;						// high seconds
	
	clock_template[10] = (m % 10) + 48; m /= 10;			// low minutes
	clock_template[9] = (m % 10) + 48;						// high minutes
	
	clock_template[7] = (h % 10) + 48; h /= 10;				// low hours
	clock_template[6] = (h % 10) + 48;						// high hours
	
	
	lcd.SendCommand(LCD::FIRST_ROW);
	lcd.WriteString(clock_template, 18); // write timer template with time inserted. 16 characters so that it doesn't overflow into other templates
}

void Program::TimerDraw() {
	// char timer_template[16] = {'T', 'i', 'm', 'e', 'r', ':', ' ', '0', '0', ':', '0', '0', ':', '0', '0', '0'};
	int temp_time = floor(timer);
	int ms = ((int)(timer * 1000)) % 1000;
	int seconds = temp_time % 60;
	int minutes = (temp_time / 60) % 60;
	
	timer_template[15] = (ms % 10) + 48; ms /= 10;				// low ms
	timer_template[14] = (ms % 10) + 48; ms /= 10;				// mid ms
	timer_template[13] = (ms % 10) + 48;						// mid ms
	
	timer_template[11] = (seconds % 10) + 48; seconds /= 10;	// low seconds
	timer_template[10] = (seconds % 10) + 48;					// high seconds
	
	timer_template[8] = (minutes % 10) + 48; minutes /= 10;		// low minutes
	timer_template[7] = (minutes % 10) + 48;					// high minutes
	
	
	lcd.SendCommand(LCD::FIRST_ROW);
	lcd.WriteString(timer_template, 18); // write timer template with time inserted. 16 characters so that it doesn't overflow into other templates
}

////////////////////////////////////////////////////////////////////////

// interrupts every 1.6ms
void Program::FastInterrupt() {
	buttons.Update();
}

// interrupts every 0.01024s
void Program::ClockInterrupt() {
	if (clock_on) {
		clock_time += 0.01024; // add delta to the time
	}
	
	if (state == 1 && timer_on) { // if state is timer and it is on
		timer += 0.01024;
	}
	
	// if we haven't updated redrawn the lcd in a while, redraw it and remember when
	if (lcd_update_counter <= clock_time) {
		switch (state) {
			case 0: lcd_update_counter += LCD_CLOCK_UPDATE_INTERVAL; break;
			case 1: lcd_update_counter += LCD_TIMER_UPDATE_INTERVAL; break;
		}
		
		Draw();
	}
}