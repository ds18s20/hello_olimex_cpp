//============================================================================
// Name        : main.cpp
// Author      : Dima
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <unistd.h>

#include "statemachine.h"
using namespace std;

#define STATEMACHNE_TIMEOUT		1


StateMachine stateMachine;

int main()
{
	cout << "!!!Hello World!!!" << endl;

	stateMachine.startms(STATEMACHNE_TIMEOUT);
	stateMachine.run();


	while(1)
	{

	}

	return 0;
}
