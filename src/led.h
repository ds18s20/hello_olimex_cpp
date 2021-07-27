/*
 * led.h
 *
 *  Created on: Jul 22, 2021
 *      Author: dima
 */

#ifndef LED_H_
#define LED_H_
#include <stdint.h>
#include "gpio.h"

class Led
{
public:
	Led(){state = 0;}
	~Led(){}
	void init(int b, int p,Gpio *g)
	{
		bank = b;
		pin = p;
		gpio =g;
		g->gpio_output(b, p);
	}
	void off() {state = 1; gpio->gpio_write(bank,pin,state); }
	void on() {state =0; gpio->gpio_write(bank,pin,state); }
	void toggle()
	{
		state = (~state)^1;
		gpio->gpio_write(bank,pin,state);
	}
private:
	int tag;
	int bank;
	int pin;
	int state;
	Gpio *gpio;
};



#endif /* LED_H_ */
