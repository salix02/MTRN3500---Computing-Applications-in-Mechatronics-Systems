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
#include "LMS151.h"



using namespace std;
#define MAX_HB_COUNT 40

//#define DEFAULT_PORT "2112"
#define DEFAULT_PORT "23000" // supplied in the assignment spec


//#define IP_ADDRESS "192.168.5.140" // supplied in assignment spec

#define IP_ADDRESS "192.168.1.200"

int main()
{
	SetConsoleTitle("Laser.exe");
	int timecount = 0;
	int HeartbeatCounter = 0;
	//SM block below
	//Request access to Laser mem block---------------------------------------------------------
	SMObject LaserObj(TEXT("LaserData"), sizeof(Laser));
	SMObject PMObj(TEXT("ProcessManagement"), sizeof(ProcessManagement));

	LaserObj.SMAccess();
	PMObj.SMAccess();

	ProcessManagement* PM = (ProcessManagement*)PMObj.pData; //type casting void pData to PM type
	Laser* LS = (Laser*)LaserObj.pData;

	//Any other initialisation------------------------------------------------------------------
	LMS151 LMSLaser(IP_ADDRESS, DEFAULT_PORT); //establishing connection
	
	//is this part required???
	if (LMSLaser.ConnectToServer() == 0)
		cout << "Connected to Laser. " << endl;
	
	//LMSLaser.Login();
	LMSLaser.Configure(45, 135, HALF_DEGREE);
	//LMSLaser.Logout();
	LMSLaser.LaserAuthenticate();
	


	// Tell PM that Laser is not shutdowned
	PM->ShutDown.Flags.Laser = 0;

	while (PM->ShutDown.Flags.Laser == 0)
	{
		//Read Laser---------------------------------------------------------------------------------
		//only read it once/second, to reduce processes and reduce lag
		if(timecount == 5){
			LMSLaser.GetRangeCoords(); //this will only scan once. Need to use while loop if need more scans

			//cout << LMSLaser.NumPoints << endl;

	
			//Write Laser data to Laser mem block--------------------------------------------------------
			for (int i = 0; i < LMSLaser.NumPoints; i++)
			{
				LS->x[i] = LMSLaser.RangeData[i][0];
				LS->y[i] = LMSLaser.RangeData[i][1];

				//this prints out the data
				//cout << LMSLaser.RangeData[i][0] << '\t' << LMSLaser.RangeData[i][1] << endl;
			}
			timecount = 0;
		}
		timecount++; 
		
		//Reset Laser.Heartbeat----------------------------------------------------------------------

		if (PM->HeartBeat.Flags.Laser == 1)
		{
			if (HeartbeatCounter < MAX_HB_COUNT)
			{
				HeartbeatCounter++;
			}
			else
			{
				PM->ShutDown.Flags.Laser = 1;
			}
		}
		else
		{
			HeartbeatCounter = 0;
			//Reset GPS.Heartbeat
			PM->HeartBeat.Flags.Laser = 1;
		}
		//cout << HeartbeatCounter << endl;


		if (_kbhit())  //if keyboard is hit
			break;
		Sleep(20);
	}

	// any termination --------------------------------------------------------------------------
	// End
	//_getch();
	return 0;
}

/*LMS151 LMSLaser(IP_ADDRESS, DEFAULT_PORT); //establishing connection


if (LMSLaser.ConnectToServer() == 0)
cout << "Connected to Laser. " << endl;
LMSLaser.Login();
LMSLaser.Configure(45, 135, HALF_DEGREE);
LMSLaser.Logout();
LMSLaser.GetRangeCoords(); //this will only scan once. Need to use while loop if need more scans

//this prints out the data
for (int i = 0; i < LMSLaser.NumPoints; i++)
cout << LMSLaser.RangeData[i][0] << '\t' << LMSLaser.RangeData[i][1] << endl;
// Laser.RangeData[i][0] = x  Laser.RangeData[i][1] = y

_getch();*/

//****************************************************************************************
//running just the top block of this code doesn't seem to work. 
//Try commenting everything below until return 0 and run it at uni
//****************************************************************************************

