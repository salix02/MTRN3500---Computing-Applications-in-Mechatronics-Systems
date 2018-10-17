#define _USE_MATH_DEFINES
#ifndef WIN_32_LEAN_AND_MEAN
#define WIN_32_LEAN_AND_MEAN
#endif

#include <string>
#include <iostream>
#include <sstream>
#include <math.h>
#include "LMS151.h"

#define STX "\x02"
#define ETX "\x03"

using namespace std;

LMS151::LMS151() :EthernetClient()
{
	StartAngle = -45;
	EndAngle = 225;
	Resolution = 0.5;
}

LMS151::LMS151(char * portaddress, char* port) :EthernetClient(portaddress, port)
{
	StartAngle = -45;
	EndAngle = 225;
	Resolution = 0.5;
}

LMS151::~LMS151()
{
}

void LMS151::Login()
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

void LMS151::Logout()
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


void LMS151::Configure(int startAngle, int endAngle, double resolution)
{
	string Fragments;
	string ConfigResp;
	int Return;
	char SetFreqRange[256]; // = "sMN mLMPsetscancfg +5000 +1 +5000 +450000 +135000";
	StartAngle = startAngle;
	EndAngle = endAngle;
	Resolution = resolution;
	//sprintf_s(SetFreqRange, "sMN mLMPsetscancfg +5000 +1 %+4d %+d %+d", resolution, startAngle * 10000, endAngle * 10000); //  scanning freq (+5000 is 50Hz), 1 is spec, Scan Res (), start angle, stop angle 
	//SendData(SetFreqRange, strlen(SetFreqRange));
	//ReceiveData();
	ConfigResp = RxData;
	istringstream is(ConfigResp);
	is >> Fragments >> Fragments >> hex >> Return; // refer to pg 98 of LMS151 manual. There is more data if needed to be read
	if (Return == 0)
		Error = NOERROR;
	else
		Error = SCAN_CONFIG_ERROR;
}

void LMS151::GetRangeCoords()
{
	//int Numpoints;
	double Angle = StartAngle;
	int Range;
	string Fragments;
	string ScanResp;
	char ScanOnce[256] = "sRN LMDscandata";  //refer to pg 89 for receive data from this 
											 // sRN LMDscandata only scans once. have to ask again to get more scan
	SendData(STX, 1);
	SendData(ScanOnce, strlen(ScanOnce));
	SendData(ETX, 1);
	ReceiveData();
	ScanResp = RxData;
	istringstream is(ScanResp);
	//cout << ScanResp << endl;
	for (int i = 0; i < 25; i++)
		is >> Fragments;
	is >> hex >> NumPoints;
	//cout << NumPoints << endl;
	for (int i = 0; i < NumPoints; i++)
	{
		is >> hex >> Range;
		RangeData[i][0] = Range / 1000.0 * cos(Angle*M_PI / 180.0);
		RangeData[i][1] = Range / 1000.0 * sin(Angle*M_PI / 180.0);
		Angle += Resolution / 10000.0;
		//cout << RangeData[i][0] << " " << RangeData[i][1] << endl;
	}
}

void LMS151::LaserAuthenticate()
{
	SendData("3459118\n", 8);
	ReceiveData();
	cout << RxData << endl;
}