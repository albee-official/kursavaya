#define F_CPU 8000000UL				// ����������� �������� ������� (8 ���)
#define RS 2
#define E 6

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "LCD.h"

LCD::LCD() {
	Init();
}

LCD::~LCD() {}


void LCD::Init() {
	DDRC=0b11111111;
	PORTC=0x00;
	PORTA=0b00000000;
	DDRA=0b00000100;
	PORTD=0b00000000;
	DDRD=0b01100000;

	_delay_ms(50);

	PORTA&=~(1<<RS);
	PORTC=0b00110000;
	PORTD&=~(1<<E);
	asm("nop");
	PORTD|=(1<<E);
	_delay_us(10);
	PORTD&=~(1<<E);
	_delay_ms(1);

	PORTC=0b00110000;
	PORTD&=~(1<<E);
	asm("nop");
	PORTD|=(1<<E);
	_delay_us(10);
	PORTD&=~(1<<E);
	_delay_ms(1);

	PORTC=0b00110000;
	PORTD&=~(1<<E);
	asm("nop");
	PORTD|=(1<<E);
	_delay_us(10);
	PORTD&=~(1<<E);
	_delay_ms(1);
	_delay_ms(10);
	
	// Post-Initialization: setting the thing up
	SendCommand(0x20);
	SendCommand(0x28);
	
	SendCommand(0x08);
	SendCommand(0x01);
	SendCommand(0x06);
	SendCommand(0x0C);
}

void LCD::SendCommand (unsigned char lcd) {
	unsigned char temp;	
	
	PORTA&=~(1<<RS);
	PORTD|=(1<<E);
	temp=lcd;
	PORTC=temp;
	
	asm("nop");
	PORTD&=~(1<<E);
	asm("nop");
	PORTD|=(1<<E);
	temp=lcd*16;
	PORTC=temp;
	asm("nop");
	PORTD&=~(1<<E);
	asm("nop");
	_delay_ms(2);
}

void LCD::SendData (unsigned char lcd) {
	unsigned char temp;
	
	PORTA|=(1<<RS);
	PORTD|=(1<<E);
	temp=lcd;

	PORTC=temp;
	
	asm("nop");
	PORTD&=~(1<<E);
	asm("nop");
	PORTD|=(1<<E);
	temp=lcd*16;

	PORTC=temp;
	asm("nop");
	PORTD&=~(1<<E);
	asm("nop");
	_delay_ms(2);
}

void LCD::WriteString(const char *str) {
	for (unsigned char k = 0; k < 255; k++) {
		if ( str[k]==0 )
		return;
		else SendData(str[k]);
	}
}

void LCD::WriteString(const char *str, int length) {
	if (length <= 0) return;
	for (unsigned char k = 0; k < length; k++) {
		if ( str[k]==0 ) continue;
		else SendData(str[k]);
	}
}