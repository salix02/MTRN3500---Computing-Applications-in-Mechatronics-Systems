#define _USE_MATH_DEFINES
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <math.h>

#include <fstream>
#include <tchar.h>
#include "SMObject.h"
#include "SMStruct.h"
#include "SMFcn.h"
#include "GNSS.h"

#define MAX_HB_COUNT 4

using namespace std;

#define GNSS_PORT "24000"
#define IP_ADDRESS "192.168.1.200"

int main()
{	
	SetConsoleTitle("GPS.exe");

	cout << sizeof(GPSNova) << endl;

	int HeartbeatCounter = 0;

	SMObject GPSObj(TEXT("GPSData"), sizeof(GPS));
	SMObject PMObj(TEXT("ProcessManagement"), sizeof(ProcessManagement));

	//Request access to GPS mem block
	GPSObj.SMAccess();
	PMObj.SMAccess();

	ProcessManagement* PM = (ProcessManagement*)PMObj.pData; //type casting void pData to PM type
	GPS* GPSys = (GPS*)GPSObj.pData;

	GNSS WeederGPS(IP_ADDRESS, GNSS_PORT);

	if (WeederGPS.ConnectToServer() == 0)
		cout << "Connected to GPS. " << endl;
	
	//Do as long as not GPS.shutdown
	while (PM->ShutDown.Flags.GPS == 0) // this should work right?
	//while (!PM->ShutDown.Flags.GPS != 1) //***********************I'm Very confused here, (SMStruct.h) so much dots
	{
		//Read GPS
		WeederGPS.GetData();
		//Write GPS data to GPS mem block
		GPSys->Height = WeederGPS.getHeight();
		GPSys->Easting = WeederGPS.getEasting();
		GPSys->Northing = WeederGPS.getNorthing();
		
		//check if heartbeat is 0ed, PM is alive
		if (PM->HeartBeat.Flags.GPS == 1)
		{
			if (HeartbeatCounter < MAX_HB_COUNT)
			{
				HeartbeatCounter++;
			}
			else
			{
				PM->ShutDown.Flags.GPS = 1;
			}
		}
		else
		{
			HeartbeatCounter = 0;
			//Reset GPS.Heartbeat
			PM->HeartBeat.Flags.GPS = 1;
		}
		
		//cout << HeartbeatCounter << endl;

		//cout << GPSys->Easting << " " << GPSys->Northing << " " << GPSys->Height << endl;
		if (_kbhit())  //if keyboard is hit
			break;
		//cout << "GPS is running " << endl;
		//cout << PM->ShutDown.Flags.GPS << endl;


		

		Sleep(20);

		//Testing purposes
		//Sleep(100);

	}
	
	return 0;
}


//Request access to GPS mem block
//Any other initialisation
//Do as long as not GPS.shutdown
	//Read GPS
	//Write GPS data to GPS mem block
	//Reset GPS.Heartbeat
// any termination
// End