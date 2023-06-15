
#ifndef __PROGRAM_H__
#define __PROGRAM_H__

#include "../LCD/LCD.h"
#include "../Buttons/Buttons.h"

#include <string.h>
using namespace std;

class Program {
public:
	LCD lcd;
	Buttons buttons;
	
	// -1: initialization
	//  0: clock
	//  1: timer
	unsigned char state = -1;
	bool timer_on = false;
	bool clock_on = true;
	
	unsigned int cached_seconds = 0;
	unsigned int cached_minutes = 0;
	unsigned int cached_hours = 0;
	
	unsigned int change_digit_index = 0;
	
	long double clock_time = 0;
	long double timer = 0;
	long double lcd_update_counter = 0;

	// Methods
	
	Program();
	~Program();
	
	void Update();
	void Draw();
	
	void InitPorts();
	void InitClockInterrupt();
	void InitFastInterrupt();
	
	void ClockInterrupt();
	void FastInterrupt();
	
	void ClockUpdate();
	void TimerUpdate();
	
	void ClockDraw();
	void TimerDraw();
	void SetTimeDraw();
	void BottomDraw();
};

#endif
