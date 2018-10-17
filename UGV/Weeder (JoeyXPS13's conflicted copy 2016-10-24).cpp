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

void Weeder::Login()
{
	string Fragments;
	string LoginResp;
	int Return;
	char Login[256] = "sMN SetAccessMode 03 F4724744";
	SendData(Login, strlen(Login));
	ReceiveData();
	LoginResp = RxData;
	istringstream is(LoginResp);
	is >> Fragments >> Fragments >> hex >> Return;
	if (Return == 0)
		Error = LOGIN_FAILED;
	else
		Error = NOERROR;
}

void Weeder::Logout()
{
	string Fragments;
	string LogoutResp;
	int Return;
	char Logout[256] = "sMN Run";
	SendData(Logout, strlen(Logout));
	ReceiveData();
	LogoutResp = RxData;
	istringstream is(LogoutResp);
	is >> Fragments >> Fragments >> hex >> Return;
	if (Return == 0)
		Error = LOGOUT_FAILED;
	else
		Error = NOERROR;
}


void Weeder::UGVAuthenticate()
{
	SendData("3459118\n", 8);
	ReceiveData();
	cout << RxData << endl;
}

void Weeder::sendCarData(double steer, double speed)
{
	char str[32]; // to store speed and steering
	
	sprintf(str, "#%f %f %d#", steer*40, speed, controlFlag);
	SendData(str, strlen(str));

	controlFlag = !controlFlag;
}