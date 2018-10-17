#define _USE_MATH_DEFINES
#ifndef WIN_32_LEAN_AND_MEAN
#define WIN_32_LEAN_AND_MEAN
#endif

#include <string>
#include <iostream>
#include <sstream>
#include <math.h>
#include "Weeder.h"

#define STX "\x02"
#define ETX "\x03"

using namespace std;

Weeder::Weeder() :EthernetClient()
{
	controlFlag = 0;
	speed = 0;
	steer = 0;
}

Weeder::Weeder(char * portaddress, char* port) :EthernetClient(portaddress, port)
{
	controlFlag = 0;
	speed = 0;
	steer = 0;
}

Weeder::~Weeder()
{
}

void Weeder::UGVAuthenticate()
{
	SendData("3459118\n", 8);
	ReceiveData();
	cout << RxData << endl;
}

void Weeder::sendCarData(double steer, double speed)
{
	
	sprintf(Buffer, "#%f %f %d#", steer*40, speed, controlFlag);
	SendData(Buffer, strlen(Buffer));

	controlFlag = !controlFlag;
}