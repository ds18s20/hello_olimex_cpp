/*
 * gpio.h
 *
 *  Created on: Jul 22, 2021
 *      Author: dima
 */

#ifndef GPIO_H_
#define GPIO_H_

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

#define GPIO_BASE 0x80018000
#define GPIO_WRITE_PIN(gpio,value) GPIO_WRITE((gpio)>>5, (gpio)&31, value)
#define GPIO_WRITE(bank,pin,value) (gpio_mmap[0x140+((bank)<<2)+((value)?1:2)] = 1<<(pin))
#define GPIO_READ_PIN(gpio) GPIO_READ((gpio)>>5, (gpio)&31)
#define GPIO_READ(bank,pin) ((gpio_mmap[0x180+((bank)<<2)] >> (pin)) & 1)

static int *gpio_mmap;

class Gpio
{
public:
	Gpio()
	{
		//gpio_map();
	}
	~Gpio(){}
	static int *gpio_map()
	{
		int fd;
		if (gpio_mmap != 0)
			return 0;
		fd = open("/dev/mem", O_RDWR);
		if( fd < 0 )
		{
			perror("Unable to open /dev/mem");
			fd = 0;
		}

		gpio_mmap = (int *)mmap(0, 0xfff, PROT_READ|PROT_WRITE, MAP_SHARED, fd, GPIO_BASE);
		if( -1 == (int)gpio_mmap)
		{
			perror("Unable to mmap file");
			gpio_mmap = 0;
		}
		if( -1 == close(fd))
			perror("Couldn't close file");
		fd=0;
	}

	void *gpio_rd(long offset) {
		return (int *)gpio_mmap[offset/4];
	}

	void gpio_wr(long offset, long value) {
		gpio_mmap[offset/4] = value;
	}

	void gpio_output(int bank, int pin) {
		gpio_mmap[0x1C1 + (bank*4)] = 1 << pin;
	}

	void gpio_input(int bank, int pin)
	{
		gpio_mmap[0x1C2 + (bank*4)] = 1 << pin;
	}

	void gpio_write(int bank, int pin, int state)
	{
		GPIO_WRITE(bank,pin,state);
	}

private:

};

//	GPIO_WRITE(0,0,0);		//con1: 12- bad
//	GPIO_WRITE(0,1,0);		//con1: 13
	//GPIO_WRITE(0,2,0);		//con1:	14
/*	GPIO_WRITE(0,3,0);		//con1: 21- bad
	GPIO_WRITE(0,4,0);		//con1: 22
	GPIO_WRITE(0,5,0);		//con1:	23
	GPIO_WRITE(0,6,0);		//con1: 24
	GPIO_WRITE(0,7,0);		//con1:	25

	GPIO_WRITE(0,16,0);		//con1: 26
	GPIO_WRITE(0,17,0);		//con1:	27
	GPIO_WRITE(0,23,0);		//con1:	23
	GPIO_WRITE(0,25,0);		//con1:	24*/




#endif /* GPIO_H_ */
