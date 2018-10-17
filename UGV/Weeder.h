#ifndef WEEDER_H
#define WEEDER_H

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif // !WIN32_LEAN_AND_MEAN

#include <EthernetClient.h>

#define LOGIN_FAILED -1
#define LOGOUT_FAILED -2
#define SCAN_CONFIG_ERROR -3


class Weeder : public EthernetClient
{
private:
	double steer;
	double speed;
	bool controlFlag;
	char Buffer[128]; // to store speed and steering

public:
	Weeder();
	Weeder(char * portaddress, char * port);
	~Weeder();
	void UGVAuthenticate();
	void sendCarData(double steer, double speed);
};

#endif