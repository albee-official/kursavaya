

#define F_CPU 8000000UL				// ����������� �������� ������� (8 ���)
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
	asm ("cli");			//������� ���������� "������ ����������"
	
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
						��������� ����� ����� A;
						������� ����� ����� A �������� �� ���� - ������� �� ����� 
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
	OCR1A=319;			// ����� 320-1
	TCCR1A=0b00000000;	// ����� ���
	TCCR1B=0b00001100;	// �������� �� 256
	TIFR=0b00111111;	// ������� ������ ��������/���������
	TIMSK=0b000010010;	// ���������� ���������� �� �/�0 � �/�1
}

void InitFastInterrupt() {
	TCNT0=0b00000000;
	OCR0=49; //   ����� 50-1
	TCCR0=0b00001100;// �������� �� 256
}

// ���������� ���������� ����������� �������-�������� 0 (�������� ���������� 1.6 ��)
ISR (TIMER0_COMP_vect) {
	program.FastInterrupt();
}

// ���������� ���������� ����������� �������-�������� 1 (�������� ���������� 0,001024�) - ������������ ���������!!!
ISR (TIMER1_COMPA_vect) {
	program.ClockInterrupt();
}