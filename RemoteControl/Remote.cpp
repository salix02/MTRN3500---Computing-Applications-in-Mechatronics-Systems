//Request access to Remote mem block
//Any other initialisation
//Do as long as not Remote.shutdown
	//Read Xbox
	//Write data to Remote mem block
	//Reset Remote.Heartbeat
// any termination
// End

#include <Windows.h>
#include <iostream>
#include <iomanip> // for setprecision
#include <Xinput.h>
#include <Controller.h>
#include <iostream>
#include <fstream>
#include <tchar.h>
#include "SMObject.h"
#include "SMStruct.h"
#include <conio.h>

#define VIBRATE_SPEED 50000
#define DEADZONE 0.5

#define MAX_HB_COUNT 150


using namespace std;

int main(int argc, char *argv) {
	SetConsoleTitle(TEXT("Remote.exe"));
	Controller C1(0), C2(1);
	int controlNumber;
	int controllerconnected = 0;
	bool Quit = false;
	bool RemConnectFlag;
	int HeartbeatCounter = 0;

	//instantiating SMObject
	SMObject RemoteObj(TEXT("RemoteControl"), sizeof(Remote));
	SMObject PMObj(TEXT("ProcessManagement"), sizeof(ProcessManagement));

	//Accessing SM block
	RemoteObj.SMAccess();
	PMObj.SMAccess();

	//obtaining pointer to SM block
	ProcessManagement* PM = (ProcessManagement*)PMObj.pData;
	Remote* REM = (Remote*)RemoteObj.pData;

	if (C1.checkConnect())
	{
		RemConnectFlag = TRUE;
	}
	else
	{
		RemConnectFlag = FALSE;
	}

	controlNumber = C1.connectionInfo();
	

	if (PM->ShutDown.Flags.Remote == 0) {
		cout << "Remote Control is running " << endl;
	}

	while (PM->ShutDown.Flags.Remote == 0)
	{
		//check if heartbeat is 0ed, PM is alive
		if (HeartbeatCounter < MAX_HB_COUNT)
		{
			HeartbeatCounter++;
		}
		else
		{
			break;
		}

		//update heartbeat

		if (PM->HeartBeat.Flags.Remote == 1)
		{
			if (HeartbeatCounter < MAX_HB_COUNT)
			{
				HeartbeatCounter++;
			}
			else
			{
				PM->ShutDown.Flags.Remote = 1;
			}
		}
		else
		{
			HeartbeatCounter = 0;
			//Reset GPS.Heartbeat
			PM->HeartBeat.Flags.Remote = 1;
		}
		//cout << HeartbeatCounter << endl;


		if (C1.getConnectStatus() == false)
		{
			REM->SerSpeed = 0;
			REM->SetSteering = 0;
			if (RemConnectFlag != FALSE)
				cout << "Depressedstick Disconnected" << endl;
			RemConnectFlag = FALSE;
		}
		else if(RemConnectFlag != TRUE)
		{
			cout << "Joystick Connected" << endl;
			RemConnectFlag = TRUE;
		}

		if (_kbhit())  //if keyboard is hit
			break;
		
		C1.getState();
		Sleep(20);

		REM->SerSpeed = (double)C1.leftThumbAnalogY();
		REM->SetSteering = -(double)C1.rightThumbAnalogX();
		REM->Terminate = C1.A();
		//cout << REM->Terminate << endl;
		//cout << REM->SetSteering << '\t' << REM->SerSpeed << endl;
	}
	
	return 0;
}