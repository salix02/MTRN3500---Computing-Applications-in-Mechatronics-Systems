#define _USE_MATH_DEFINES
#ifndef WIN_32_LEAN_AND_MEAN
#define WIN_32_LEAN_AND_MEAN
#endif

#include <string>
#include <iostream>
#include <sstream>
#include <math.h>

#include "GNSS.h"
#define CRC32_POLYNOMIAL		0xEDB88320L
#define ENDOFDATA 3984

using namespace std;

GNSS::GNSS() :EthernetClient()
{
	BytePtr = (unsigned char*)&GPSRec; //this should be pointing to 1st address of the struct right?
	
}

GNSS::GNSS(char * portaddress, char* port) :EthernetClient(portaddress, port)
{
	BytePtr = (unsigned char*)&GPSRec;
}

GNSS::~GNSS()
{
}

unsigned char GNSS::ReceiveByte(int i)
{
	return RxData[i];
}

void GNSS::GetData()
{
	bool EndOfPath = false;
	GPSRec.FullHeader = 0x00;
	int counter = 0;

	//receive data from server, fills in RxData
	ReceiveData();

	while (!EndOfPath)
	{
		// Trapping the header
		while (1)//make sure this is checked
		{
			Byte = ReceiveByte(counter); //check the header we are receiving from the serial port
			GPSRec.FullHeader <<= 8;
			GPSRec.FullHeader |= Byte;
			counter++;
			//
			//cout << hex << GPSRec.FullHeader << endl;
			//checking if the start of stream is found
			if (GPSRec.FullHeader == 0xAA44121C)
			{
				break; // Header found
			}
			if (counter == ENDOFDATA)
			{
				cout << "No Header found" << endl;
				break;

			}
			
		}
		if (counter == ENDOFDATA) 
		{
			break;
		}
		
		//filling in Entire structure in struct GPS, particularly northing, easting, height and Checksum
		for (int i = 0; i < 112; i++)
		{
			//ptr to the first address of struct, + 4 cs fullheader is filled, + i for each iteration of byte store
			*(BytePtr + i) = ReceiveByte(i); //i = 0 start of Rejects1	
		}
		cout << endl;

		cout << "Easting: " << GPSRec.Easting << endl;	
		cout << "Northing: " << GPSRec.Northing << endl;
		cout << "Height: " << GPSRec.Height << endl;

		cout << "actual checksum:" << "\t" << GPSRec.Checksum << endl;
		cout << "calculated checksum:" << "\t" << CalculateBlockCRC32((sizeof(GPSNova) - 4), BytePtr) << endl;
		//check checksum
		if (GPSRec.Checksum == CalculateBlockCRC32((sizeof(GPSNova) - 4), BytePtr))
		{
			cout << "Checksum validated" << endl;
		}
		else {
			cout << "Checksum incorrect" << endl;
		}

		EndOfPath = true;
	}

	//reset the fullheader for next round of searches
}

double GNSS::getNorthing()
{
	return GPSRec.Northing;
}

double GNSS::getEasting()
{
	return GPSRec.Easting;
}

double GNSS::getHeight()
{
	return GPSRec.Height;
}

unsigned long GNSS::CRC32Value(int i)
{
	int j;
	unsigned long ulCRC;
	ulCRC = i;
	for (j = 8; j > 0; j--)
	{
		if (ulCRC & 1)
			ulCRC = (ulCRC >> 1) ^ CRC32_POLYNOMIAL;
		else
			ulCRC >>= 1;
	}
	return ulCRC;
}
/* --------------------------------------------------------------------------
Calculates the CRC-32 of a block of data all at once
-------------------------------------------------------------------------- */
unsigned long GNSS::CalculateBlockCRC32(unsigned long ulCount, 	unsigned char *ucBuffer) /* Data block */
{
	unsigned long ulTemp1;
	unsigned long ulTemp2;
	unsigned long ulCRC = 0;
	while (ulCount-- != 0)
	{
		ulTemp1 = (ulCRC >> 8) & 0x00FFFFFFL;
		ulTemp2 = CRC32Value(((int)ulCRC ^ *ucBuffer++) & 0xff);
		ulCRC = ulTemp1 ^ ulTemp2;
	}
	return(ulCRC);
}

//Note: Data Block ucBuffer should contain all data bytes of the full data record except the checksum bytes.