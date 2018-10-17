#define _USE_MATH_DEFINES
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

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

//how many programs being run
#define NUM_UNITS 6
//#define CRITICAL_MASK 0x0F // for which modules we are interested in running. Refer to UnitFlags for which one to mask
#define CRITICAL_MASK 0x00							//currently running GPS, Sim and Laser


#define NOT_CRITICAL_MASK 0x70
#define MAX_HB_COUNT 150

// check if processes are running
bool IsProcessRunning(const char *processName)
{

	bool exists = false;
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);

	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	if (Process32First(snapshot, &entry))
		while (Process32Next(snapshot, &entry))
			if (!_stricmp(entry.szExeFile, processName))
				exists = true;

	CloseHandle(snapshot);
	return exists;
}

int WatchDog();

int main()
{
	SetConsoleTitle("ProcessManagement.exe");
	int HeartbeatCounter = 0;
	int NotCritHBCounter = 0;
	
	STARTUPINFO s[10];
	PROCESS_INFORMATION p[10];

	
	//Request access to PM  mem block
	//SMObject SMemObj(TEXT("SharedMemory"), sizeof(SharedMem);
	SMObject LaserObj(TEXT("LaserData"), sizeof(Laser));
	SMObject RemoteObj(TEXT("RemoteControl"), sizeof(Remote));
	SMObject GPSObj(TEXT("GPSData"), sizeof(GPS));
	SMObject PMObj(TEXT("ProcessManagement"), sizeof(ProcessManagement));
	SMObject SimObj(TEXT("Simulator"), sizeof(Simulator));
	//SMObject CamObj(TEXT("Camera"), sizeof(Camera)) nothing to be shared


	//create SM objects. This will be handled by SharedMemory   module
	PMObj.SMCreate();
	LaserObj.SMCreate();
	RemoteObj.SMCreate();
	GPSObj.SMCreate();
	SimObj.SMCreate();

	// Request access to GPS mem block
	PMObj.SMAccess();
	RemoteObj.SMAccess();

	ProcessManagement* PM = (ProcessManagement*)PMObj.pData; //can make a template so don't have to specify this variable
	Remote* REM = (Remote*)RemoteObj.pData;
															 // Start up all modules
	PM->ShutDown.Flags.Laser = 0;
	PM->ShutDown.Flags.Simulator = 0;
	PM->ShutDown.Flags.GPS = 0;
	PM->ShutDown.Flags.Camera = 0;
	PM->ShutDown.Flags.Remote = 0;
	PM->ShutDown.Flags.UGV = 0;
	PM->ShutDown.Flags.PM = 0;


	// Start up sequence
	TCHAR* Units[10] = //Max processes is 10
	{
		TEXT("Laser.exe"),
		TEXT("RemoteControl.exe"),
		TEXT("Camera.exe"),
		TEXT("Simulator.exe"),
		TEXT("UGV.exe"),
		TEXT("GPS.exe")
		
	};

	//Run all other processes ========
	for (int i = 0; i < NUM_UNITS; i++)
	{
		//Check if each process is running
		if (!IsProcessRunning(Units[i]))
		{
			ZeroMemory(&s[i], sizeof(s[i]));
			s[i].cb = sizeof(s[i]);
			ZeroMemory(&p[i], sizeof(p[i]));
			//start child processes

			if (!CreateProcess(NULL,	// No module name (use command line)
				Units[i],				// Command line
				NULL,					// Process handle not ingeritable
				NULL,					// Thread handle not inheritable
				FALSE,					// Set handle inheritance to FALSE
				CREATE_NEW_CONSOLE,		// No creation flags
				NULL,					// Use parent's environment block
				NULL,					// Use parent's starting directory
				&s[i],					// pointer to STARTUPINFO structure
				&p[i])					// pointer to PROCESS_INFORMATION structure
				)
			{
				printf("%s failed (%d).\n", Units[i], GetLastError());
				_getch();
				return -1;
			}
		}
		cout << "Started: " << Units[i] << endl;
		Sleep(100);
	}
	
	

	// DO as long as not PM.ShutDown
	while (!PM->ShutDown.Flags.PM == 1)
	{
		// Check heartbeats
		if ((PM->HeartBeat.Status & CRITICAL_MASK) != CRITICAL_MASK) // if three of the module runs it will be 4a, (the number for CRITICAL_MASK)
			HeartbeatCounter++; // when this counter = CRITICAL_MASK, everything is running fine
		else
			HeartbeatCounter = 0;

		if (HeartbeatCounter >= MAX_HB_COUNT)
		{
			PM->ShutDown.Status = 0xFFFF; //shutting down all the modules
			break;
		}
		
		if ((PM->HeartBeat.Status & NOT_CRITICAL_MASK) != NOT_CRITICAL_MASK) // if three of the module runs it will be 4a, (the number for CRITICAL_MASK)
			NotCritHBCounter++; // when this counter = CRITICAL_MASK, everything is running fine
		else
			NotCritHBCounter = 0;
	

		//restarting some processes
		if (NotCritHBCounter >= MAX_HB_COUNT)
		{
			//Make sure that car doesn't move when RemoteControl.exe runs
			if (!IsProcessRunning(TEXT("RemoteControl.exe")))
			{
				REM->SerSpeed = 0;
				REM->SetSteering = 0;
			}
			
			//restart some processes
			for (int i = 3; i < NUM_UNITS; i++)
			{
				//Check if each process is running
				if (!IsProcessRunning(Units[i]))
				{
					ZeroMemory(&s[i], sizeof(s[i]));
					s[i].cb = sizeof(s[i]);
					ZeroMemory(&p[i], sizeof(p[i]));
					//start child processes

					if (!CreateProcess(NULL,	// No module name (use command line)
						Units[i],				// Command line
						NULL,					// Process handle not ingeritable
						NULL,					// Thread handle not inheritable
						FALSE,					// Set handle inheritance to FALSE
						CREATE_NEW_CONSOLE,		// No creation flags
						NULL,					// Use parent's environment block
						NULL,					// Use parent's starting directory
						&s[i],					// pointer to STARTUPINFO structure
						&p[i])					// pointer to PROCESS_INFORMATION structure
						)
					{
						printf("%s failed (%d).\n", Units[i], GetLastError());
						_getch();
						return -1;
					}
					//cout << "Restarted: " << Units[i] << endl;
					//Sleep(100);
				}
				
			}
		}

		// Maintain watch of heartbeats

		//check if the heartbeats are happeningw
		system("cls");
		cout << "Process Management Heart Beat is:\t" << PM->HeartBeat.Flags.PM << endl;
		cout << "Laser Heart Beat is:\t\t\t" << PM->HeartBeat.Flags.Laser << endl;
		cout << "Remote Control Heart Beat is:\t\t" << PM->HeartBeat.Flags.Remote << endl;
		cout << "Camera Heart Beat is:\t\t\t" << PM->HeartBeat.Flags.Camera << endl;
		cout << "Simulator Heart Beat is:\t\t" << PM->HeartBeat.Flags.Simulator << endl;
		cout << "UGV Heart Beat is:\t\t\t" << PM->HeartBeat.Flags.UGV << endl;
		cout << "GPS Heart Beat is:\t\t\t" << PM->HeartBeat.Flags.GPS << endl;
		cout << "Heart Beat Status is:\t\t\t" << (PM->HeartBeat.Status&0x0F) << endl;

		// Force heartbeat to ZERO
		PM->HeartBeat.Status = 0x00;


		// Manage shutdown/start up
		if (_kbhit())
			break;
		
		PM->HeartBeat.Flags.PM = 1; // This line is not what is required in assignment

		WatchDog(); //Watches over PM if it fails, through hardware.

		if (REM->Terminate == TRUE)
			PM->ShutDown.Status = 0xFFFF;;

		Sleep(20);
		
	}

	PM->ShutDown.Status = 0xFFFF;

	return 0;
}

int WatchDog()
{
	return 0;
}
//Any other initialisation
//Do as long as not PM.shutdown
	//Check heartbeats
	//Maintain watch of heartbeats (some might not be 1 yet due to speed of process)
	//Force heartbeat to ZERO
	//Manage shutdown/start up
	//Reset PM.Heartbeat
// any termination
// End
/*
void restartProcesses(TCHAR* Units)
{

		//Check if each process is running
		if (!IsProcessRunning(Units))
		{
			ZeroMemory(&s, sizeof(s));
			s.cb = sizeof(s);
			ZeroMemory(&p, sizeof(p));
			//start child processes

			if (!CreateProcess(NULL,	// No module name (use command line)
				Units,				// Command line
				NULL,					// Process handle not ingeritable
				NULL,					// Thread handle not inheritable
				FALSE,					// Set handle inheritance to FALSE
				CREATE_NEW_CONSOLE,		// No creation flags
				NULL,					// Use parent's environment block
				NULL,					// Use parent's starting directory
				&s[i],					// pointer to STARTUPINFO structure
				&p[i])					// pointer to PROCESS_INFORMATION structure
				)
			{
				printf("%s failed (%d).\n", Units[i], GetLastError());
			}
		}
		cout << "Retarted: " << Units << endl;
		Sleep(100);
}*/