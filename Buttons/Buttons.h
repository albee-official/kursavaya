
#ifndef __BUTTONS_H__
#define __BUTTONS_H__

class Buttons {
protected:
	unsigned char gread_key = 0, gkey_up_counter = 0, gkey_status = 0, gkey_down_counter = 0;
	unsigned char gkey = 0;
	
public:
	Buttons();
	~Buttons();
	
	void Update();
	bool IsPressed();
	
	bool IsDown(int i);
};

#endif
