#define F_CPU 8000000UL
#define RS 2
#define E 6

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "Buttons.h"

Buttons::Buttons() {
}

Buttons::~Buttons() { }

void Buttons::Update() {
	
	gread_key=PIND; // read the PIND port
	gread_key=(~gread_key)&0x0f;
	
	if (
		(gread_key!=0) && 
		(gkey_status==0)
	)
	{
		gkey_up_counter++;
		
		if (gkey_up_counter > 10) // Simple chatter suppression. the button needs to be held for 10 Update cycles for it to register.
		{
			gkey_up_counter=0;
			gkey_status=0b11000000;
			gkey=0;
			
			if ((gread_key&0x01)!=0) 
				gkey = 0;
			if ((gread_key&0x02)!=0) 
				gkey = 1;
			if ((gread_key&0x04)!=0) 
				gkey = 2;
			if ((gread_key&0x08)!=0) 
				gkey = 3;
		}
	}
	
	else gkey_up_counter = 0;
	
	if ((gread_key==0)&&((gkey_status&0b01000000))!=0)
	{
		gkey_down_counter++;
		
		if (gkey_down_counter>10)
		{
			gkey_down_counter=0;
			gkey_status=gkey_status&0b10000000;
			
		}
	}
	
	else gkey_down_counter=0;
}

bool Buttons::IsPressed() {
	if ( (gkey_status&0b10000000)!=0 ) {
		gkey_status=gkey_status&0b01000000;
		return true;
	}
	
	return false;
}

bool Buttons::IsDown(int i) {
	return gkey == i;
}