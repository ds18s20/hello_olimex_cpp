//============================================================================
// Name        : hello_olimex_cpp.cpp
// Author      : Dima
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "gpio-mmap.h"
#include <unistd.h>
using namespace std;


int main() {
	cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!

	gpio_map();
	gpio_output(0,0);	//bad
	gpio_output(0,1);
	gpio_output(0,1);
	gpio_output(0,2);
	gpio_output(0,3);	//bad
	gpio_output(0,4);
	gpio_output(0,5);
	gpio_output(0,6);
	gpio_output(0,7);
	gpio_output(0,16);
	gpio_output(0,17);
	gpio_output(0,23);
	gpio_output(0,25);

	while(1)
	{
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

		sleep(1);

	//	GPIO_WRITE(0,0,1);		//con1: 12
	//	GPIO_WRITE(0,1,1);		//con1: 13
		GPIO_WRITE(0,2,1);		//con1:	14
	/*	GPIO_WRITE(0,3,1);		//con1: 21
		GPIO_WRITE(0,4,1);		//con1: 22
		GPIO_WRITE(0,5,1);		//con1:	23
		GPIO_WRITE(0,6,1);		//con1: 24
		GPIO_WRITE(0,7,1);		//con1:	25

		GPIO_WRITE(0,16,1);		//con1: 26
		GPIO_WRITE(0,17,1);		//con1:	27
		GPIO_WRITE(0,23,1);		//con1:	23
		GPIO_WRITE(0,25,1);		//con1:	24*/
		sleep(1);
	}

	return 0;
}
