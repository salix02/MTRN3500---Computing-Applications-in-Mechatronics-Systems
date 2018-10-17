#ifndef CONTROllER_H
#define CONTROLLER_H

class Controller{
protected:
	XINPUT_BATTERY_INFORMATION BatteryInformation;
	XINPUT_STATE State;
	XINPUT_VIBRATION Vibration;
	int controlindex;
public:
	Controller();
	Controller(int);
	void displayBatteryInfo();
	bool checkConnect();
	bool getConnectStatus();
	int connectionInfo();
	int countConnectedController();
	void Controller::getState();
	void checkPressed();
	void makeVibrate();
	void stopVibrate(int);

	bool up();
	bool down();
	bool left();
	bool right();
	bool start();
	bool back();
	bool leftThumb();
	bool rightThumb();
	bool leftShoulder();
	bool rightShoulder();
	bool A();
	bool B();
	bool X();
	bool Y();

	double leftTrigger();
	double rightTrigger();

	double leftThumbAnalogX();
	double leftThumbAnalogY();
	double rightThumbAnalogX();
	double rightThumbAnalogY();


	int getRightTrigger();


	void vibration();
};
#endif//!CONTROllER_H