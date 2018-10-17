#ifndef SMSTRUCT_H
#define SMSTRUCT_H

#include <Windows.h>
#include <tchar.h>

struct GPS
{
	double Northing;
	double Easting;
	double Height;
	double Heading;
	unsigned char GPSQuality;
	double GPSTimeStamp;
};

struct Laser
{
	double x[1100];
	double y[1100];
	double LaserTimeStamp;
};


struct UnitFlags
{
	unsigned short PM : 1,
		Laser : 1,
		Remote : 1,
		Camera : 1,
		Simulator : 1,
		UGV : 1,
		GPS : 1,
		SM : 1,
		Unused : 8;
};

union ExecFlags
{
	UnitFlags Flags;
	unsigned short Status;
};

struct ProcessManagement
{
	ExecFlags HeartBeat;
	ExecFlags ShutDown;
};

struct Remote
{
	double SetSteering;
	double SerSpeed;
	unsigned char UGV_SIM; //Choose UGV or simulator
	int Terminate;
};

struct Simulator
{
	double X;
	double Y;
	double Heading;
};

struct UGV
{
	double X;
	double Y;
	double Heading;
};


#endif