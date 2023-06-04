

#define F_CPU 8000000UL				// Определение тактовой частоты (8 МГц)
#define RS 2
#define E 6

#include <avr/io.h>
#include <util/delay.h>				
#include <avr/interrupt.h>

// Custom Includes
#include "LCD/LCD.h"
#include "Program/Program.h"

void InitPorts();
void InitClockInterrupt();
void InitFastInterrupt();

Program program;

int main(void) {
	asm ("cli");			//Команда ассемблера "запрет прерываний"
	
	InitPorts();
	InitClockInterrupt();
	InitFastInterrupt();
	
	program = Program();
	
	asm ("sei");
	
	while (true) {
		program.Update();
	}
}

void InitPorts() {
	PORTA=0b00000000;
	DDRA=0x0F;		/*	Data Direction Register;
						настройка линий порта A;
						старшие линии порта A работают на ввод - младшие на вывод 
					*/
	PORTC=0x00;
	DDRC=0xff;
	PORTB=0x00;
	DDRB=0xff;
	PORTD=0x00;
	DDRD=0b01100000;
}

void InitClockInterrupt() {
	TCNT1=0x0000;
	OCR1A=319;			// число 320-1
	TCCR1A=0b00000000;	// режим СТС
	TCCR1B=0b00001100;	// делитель на 256
	TIFR=0b00111111;	// очистка флагов таймеров/счетчиков
	TIMSK=0b000010010;	// разрешение прерываний от Т/С0 и Т/С1
}

void InitFastInterrupt() {
	TCNT0=0b00000000;
	OCR0=49; //   число 50-1
	TCCR0=0b00001100;// делитель на 256
}

// Обработчик прерываний компаратора таймера-счетчика 0 (интервал прерываний 1.6 мс)
ISR (TIMER0_COMP_vect) {
	program.FastInterrupt();
}

// Обработчик прерываний компаратора таймера-счетчика 1 (интервал прерываний 0,001024с) - максимальный приоритет!!!
ISR (TIMER1_COMPA_vect) {
	program.ClockInterrupt();
}