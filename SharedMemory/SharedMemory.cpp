#include <Windows.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <math.h>
#include <string>
#include <stdio.h>
#include <conio.h>
#include <time.h>
#include <tchar.h>
#include <dos.h>
#include <stdlib.h>
#include <cstdio>
#include <tlhelp32.h>
#include "SMObject.h"
#include "SMStruct.h"

using namespace std;


int main()
{
	//SMObject SMemObj(TEXT("SharedMemory"), sizeof(SharedMem);
	SMObject LaserObj(TEXT("LaserData"), sizeof(Laser));
	SMObject RemoteObj(TEXT("RemoteControl"), sizeof(Remote));
	SMObject GPSObj(TEXT("GPSData"), sizeof(GPS));
	SMObject PMObj(TEXT("ProcessManagement"), sizeof(ProcessManagement));
	SMObject SimObj(TEXT("Simulator"), sizeof(Simulator));

	//SMObject CamObj(TEXT("Camera"), sizeof(Camera)) nothing to be shared


	//create SM objects
	LaserObj.SMCreate();
	RemoteObj.SMCreate();
	GPSObj.SMCreate();
	PMObj.SMCreate();
	SimObj.SMCreate();

	// Request access to GPS mem block
	GPSObj.SMAccess();
	LaserObj.SMAccess();
	PMObj.SMAccess();
	RemoteObj.SMAccess();

	ProcessManagement* PM = (ProcessManagement*)PMObj.pData; //type casting void pData to PM type
	Remote* REM = (Remote*)RemoteObj.pData;
	GPS* GPSys = (GPS*)GPSObj.pData;
	Laser* LS = (Laser*)LaserObj.pData;

	while (PM->ShutDown.Flags.SharedMem == 0) {
		PM->HeartBeat.Flags.SharedMem = 1;
		if (_kbhit())  //if keyboard is hit
			break;
		Sleep(20);

	}

	return 0;

}