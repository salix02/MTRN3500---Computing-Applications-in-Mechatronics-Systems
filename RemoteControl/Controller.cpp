#include <Windows.h>
#include <iostream>
#include <iomanip> // for setprecision
#include <Xinput.h>
#include <Controller.h>
#define VIBRATE_SPEED 50000;
#define DEADZONE 0.5;

using namespace std;

//constructors
Controller::Controller() {
	controlindex = 0;
}

Controller::Controller(int ci) {
	controlindex = ci;
}

//Checks whether specified controller is connected
bool Controller::checkConnect() {
	XInputEnable(true);
	if (XInputGetState(controlindex, &State) == ERROR_DEVICE_NOT_CONNECTED)
	{
		cout << "Joystick not connected " << endl;
		Sleep(300);
		return false;
	}
	else
	{
		cout << "Joystick connected" << endl;
		Sleep(300);
		displayBatteryInfo();
		return true;
	}
}

bool Controller::getConnectStatus() {
	XInputEnable(true);
	if (XInputGetState(controlindex, &State) == ERROR_DEVICE_NOT_CONNECTED)
	{
		return false;
	}
	else
	{
		return true;
	}
}

//Checks how many controller is connected and which one to view status of
int Controller::connectionInfo() {
	int controlNumber = 5;
	int connected = countConnectedController();
	if (connected > 4) {
		cout << "you have connected too many controllers"; //error checking for connecting more than four controllers
	}
	cout << "There are " << connected << " controllers connected." << endl;
	Sleep(300);
	if (connected == 0) {
		cout << "please connect some jostick, I'm feeling disconnected" << endl;
		return -1;
	}
	//cout << "input controller index number (between 0-3) for status report: ";
	cout << "controller 0 status will be reported" << endl;
	controlNumber = 0;

	while (true)
	{
		//cin >> controlNumber;
		cout << endl;
		if (XInputGetState((controlNumber), &State) == ERROR_SUCCESS) // controller index 0-3 but input will be 1-4
		{
			return controlNumber;
		}
		else
		{
			cout << "please enter a connected joystick number:";
		}
		cout << endl;
	}
}

void Controller::getState() {
	XInputGetState(controlindex, &State);
}

void Controller::checkPressed() {
	bool Quit = false;
	getState();

	while (Quit != true)
	{
		if (XInputGetState(controlindex, &State) == ERROR_DEVICE_NOT_CONNECTED) {
			break;
		}
		cout << Controller::up() << " ";
		cout << Controller::down() << " ";
		cout << Controller::left() << " ";
		cout << Controller::right() << "   ";
		cout << Controller::start() << " ";
		cout << Controller::back() << "   ";
		cout << Controller::leftThumb() << " ";
		cout << Controller::rightThumb() << " ";
		cout << Controller::leftShoulder() << " ";
		cout << Controller::rightShoulder() << "   ";
		cout << Controller::A() << " ";
		cout << Controller::B() << " ";
		cout << Controller::X() << " ";
		cout << Controller::Y() << "   ";
		cout << (double)Controller::leftTrigger() << " ";
		cout << (double)Controller::rightTrigger() << "   ";
		//cout << sqrt((Controller::leftThumbAnalogX()*Controller::leftThumbAnalogX()) + (Controller::leftThumbAnalogY() + Controller::leftThumbAnalogY())) << " ";
		//cout << sqrt((Controller::rightThumbAnalogX()*Controller::rightThumbAnalogX()) + (Controller::rightThumbAnalogY() + Controller::rightThumbAnalogY())) << " ";
		cout << std::setprecision(2) << Controller::rightThumbAnalogX() << "  ";
		cout << std::setprecision(2) << Controller::rightThumbAnalogY() << "  ";
		cout << std::setprecision(2) << Controller::leftThumbAnalogX() << "  ";
		cout << std::setprecision(2) << Controller::leftThumbAnalogY() << "  ";
		vibration();
		cout << endl;

	}
}

//count the number of controllers connected
int Controller::countConnectedController()
{
	int counter = 0;
	int i = 0;
	while (i < 5) {//checks up to 5 connected
		if (XInputGetState(i, &State) == ERROR_SUCCESS)
		{
			counter++;
		}
		i++;
	}
	return counter;
}

//displays the battery info of the controller
void Controller::displayBatteryInfo()
{
	XInputGetBatteryInformation(controlindex, BATTERY_DEVTYPE_GAMEPAD, &BatteryInformation);
	switch (BatteryInformation.BatteryLevel)
	{
	case BATTERY_LEVEL_EMPTY: cout << "Battery level empty " << endl;
		break;
	case BATTERY_LEVEL_LOW: cout << "Battery level low " << endl;
		break;
	case BATTERY_LEVEL_MEDIUM: cout << "Battery level medium " << endl;
		break;
	case BATTERY_LEVEL_FULL: cout << "Battery level full" << endl;
	}
	Sleep(300);
}


void Controller::vibration() {
	XInputGetState(controlindex, &State);

	if (State.Gamepad.wButtons == XINPUT_GAMEPAD_BACK) //change the input to make things vibrate
	{
		cout << Vibration.wLeftMotorSpeed << "     " << Vibration.wRightMotorSpeed << " ";
		Vibration.wLeftMotorSpeed = VIBRATE_SPEED;
		Vibration.wRightMotorSpeed = 0;
		XInputSetState(controlindex, &Vibration); //which controller vibrates
	}
	else if (State.Gamepad.wButtons == XINPUT_GAMEPAD_START)
	{
		cout << Vibration.wLeftMotorSpeed << "     " << Vibration.wRightMotorSpeed << " ";
		Vibration.wLeftMotorSpeed = 0;
		Vibration.wRightMotorSpeed = VIBRATE_SPEED;
		XInputSetState(controlindex, &Vibration);
	}
	else if (State.Gamepad.wButtons == XINPUT_GAMEPAD_START + XINPUT_GAMEPAD_BACK)
	{
		cout << Vibration.wLeftMotorSpeed << " " << Vibration.wRightMotorSpeed << " ";
		Vibration.wLeftMotorSpeed = VIBRATE_SPEED;
		Vibration.wRightMotorSpeed = VIBRATE_SPEED;
		XInputSetState(controlindex, &Vibration);
	}

	else
	{
		cout << Vibration.wLeftMotorSpeed << "     " << Vibration.wRightMotorSpeed << "     ";;
		Vibration.wLeftMotorSpeed = 0;
		Vibration.wRightMotorSpeed = 0;
		XInputSetState(controlindex, &Vibration);
	}
}



bool Controller::up() {
	if ((State.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) != 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

bool Controller::down() {
	if ((State.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) != 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

bool Controller::left() {
	if ((State.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) != 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
bool Controller::right() {
	if ((State.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) != 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
bool Controller::start() {
	if ((State.Gamepad.wButtons & XINPUT_GAMEPAD_START) != 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
bool Controller::back() {
	if ((State.Gamepad.wButtons & XINPUT_GAMEPAD_BACK) != 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
bool Controller::leftThumb() {
	if ((State.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB) != 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
bool Controller::rightThumb() {
	if ((State.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB) != 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
bool Controller::leftShoulder() {
	if ((State.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) != 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
bool Controller::rightShoulder() {
	if ((State.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) != 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
bool Controller::A() {
	if ((State.Gamepad.wButtons & XINPUT_GAMEPAD_A) != 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
bool Controller::B() {
	if ((State.Gamepad.wButtons & XINPUT_GAMEPAD_B) != 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
bool Controller::X() {
	if ((State.Gamepad.wButtons & XINPUT_GAMEPAD_X) != 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
bool Controller::Y() {
	if ((State.Gamepad.wButtons & XINPUT_GAMEPAD_Y) != 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}


double Controller::leftTrigger() {
	//XInputGetState(controlindex, &State);
	return (double)State.Gamepad.bLeftTrigger / 255;
}

double Controller::rightTrigger() {
	//XInputGetState(controlindex, &State);
	return (double)State.Gamepad.bRightTrigger / 255;
}

/*double Controller::leftThumbAnalog(){
double normalisedLX = fmax(-1, (double)State.Gamepad.sThumbLX / 32767);
double normalisedLY = fmax(-1, (double)State.Gamepad.sThumbLY / 32767);

double deadzoneX = 0.05*normalisedLX;//5% of the motion
double deadzoneY = 0.02*normalisedLY;//2% of the motion

double lStickX = 0;
double lStickY = 0;

if (abs(normalisedLX) < deadzoneX){
lStickX = 0;
}
else
{
lStickX = normalisedLX;
}

if (abs(normalisedLY) < deadzoneY){
lStickY = 0;
}
else
{
lStickY = normalisedLY;
}
if (deadzoneX > 0)
{
lStickX = lStickX / (1 - deadzoneX);
}
if (deadzoneY > 0)
{
lStickY = lStickY / (1 - deadzoneY);
}
}*/

double Controller::leftThumbAnalogX() {
	double normalisedLX = fmax(-1, (double)State.Gamepad.sThumbLX / 32767);

	double deadzoneX = DEADZONE;

	double lStickX = 0;

	if (abs(normalisedLX) < deadzoneX) {
		lStickX = 0;
	}
	else
	{
		lStickX = (abs(normalisedLX) - deadzoneX) * (normalisedLX / abs(normalisedLX));
		lStickX = (lStickX) / (1 - deadzoneX);
	}

	return lStickX;
}
double Controller::leftThumbAnalogY() { // this works yayayayyayay

	double normalisedLY = fmax(-1, (double)State.Gamepad.sThumbLY / 32767);

	double deadzoneY = DEADZONE;

	double lStickY = 0;

	if (abs(normalisedLY) < deadzoneY) {
		lStickY = 0;
	}
	else
	{
		lStickY = (abs(normalisedLY) - deadzoneY) * (normalisedLY / abs(normalisedLY));
		lStickY = (lStickY) / (1 - deadzoneY);
	}
	return lStickY;
}



double Controller::rightThumbAnalogX() {
	double normalisedRX = fmax(-1, (double)State.Gamepad.sThumbRX / 32767);

	double deadzoneX = DEADZONE;

	double rStickX = 0;

	if (abs(normalisedRX) < deadzoneX) {
		rStickX = 0;
	}
	else
	{
		rStickX = (abs(normalisedRX) - deadzoneX) * (normalisedRX / abs(normalisedRX));
		rStickX = (rStickX) / (1 - deadzoneX);
	}

	return rStickX;
}

double Controller::rightThumbAnalogY() {
	double normalisedRY = fmax(-1, (double)State.Gamepad.sThumbRY / 32767);

	double deadzoneY = DEADZONE;

	double rStickY = 0;

	if (abs(normalisedRY) < deadzoneY) {
		rStickY = 0;
	}
	else
	{
		rStickY = (abs(normalisedRY) - deadzoneY) * (normalisedRY / abs(normalisedRY));
		rStickY = (rStickY) / (1 - deadzoneY);
	}

	return rStickY;
}
