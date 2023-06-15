
#ifndef __LCD_H__
#define __LCD_H__

#include "LCD.h"

class LCD {
public:
	static const int FIRST_ROW = 0x80;
	static const int SECOND_ROW = 0x80 + 40;
	static const int CLEAR_DISPLAY = 0x01;

	LCD();
	~LCD();
	
	void SendCommand(unsigned char lcd);
	void SendData(unsigned char lcd);
	void WriteString(const char* str);
	void WriteString(const char *str, int length);
	
protected:
	void Init();
};

#endif
