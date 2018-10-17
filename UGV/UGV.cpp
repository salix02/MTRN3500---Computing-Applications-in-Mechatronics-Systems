#define _USE_MATH_DEFINES
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <tchar.h>
#include "SMFcn.h"
#include "SMObject.h"
#include "SMStruct.h"
#include <conio.h>

#include "Weeder.h"

using namespace std;

#define UGV_PORT "25000"
#define IP_ADDRESS "192.168.1.200"

#define MAX_HB_COUNT 50

int main()
{
	int HeartbeatCounter = 0;
	
	SetConsoleTitle("UGV.exe");
	SMObject RemoteObj(TEXT("RemoteControl"), sizeof(Remote));
	SMObject PMObj(TEXT("ProcessManagement"), sizeof(ProcessManagement));

	//Request access to required mem block
	RemoteObj.SMAccess();
	PMObj.SMAccess();

	ProcessManagement* PM = (ProcessManagement*)PMObj.pData; //type casting void pData to PM type
	Remote* REM = (Remote*)RemoteObj.pData;

	Weeder UGV(IP_ADDRESS, UGV_PORT);

	if (UGV.ConnectToServer() == 0)
		cout << "Connected to UGV. " << endl;

	UGV.UGVAuthenticate();

	PM->ShutDown.Flags.UGV = 0;

	//Do as long as not UGV.shutdown
	while (PM->ShutDown.Flags.UGV == 0) 
	{
		//Reset UGV Hearbeat
		
		if (PM->HeartBeat.Flags.UGV == 1)
		{
			if (HeartbeatCounter < MAX_HB_COUNT)
			{
				HeartbeatCounter++;
			}
			else
			{
				PM->ShutDown.Flags.UGV = 1;
			}
		}
		else
		{
			HeartbeatCounter = 0;
			//Reset GPS.Heartbeat
			PM->HeartBeat.Flags.UGV = 1;
		}

		//cout << HeartbeatCounter << endl;


		if (_kbhit())  //if keyboard is hit
			break;
		//cout << "UGV is running " << endl;
		 
		UGV.sendCarData(REM->SetSteering, REM->SerSpeed);

		//cout << REM->SetSteering << endl;

		Sleep(20);

	}

	return 0;
}