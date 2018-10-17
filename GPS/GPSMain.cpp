/*#include <iostream>

using namespace std;

struct GPS
{
	// Header bytes 0xAA, 0x44, 0x12, 0x1C (number of header bytes)
	unsigned int FullHeader;
	unsigned char Rejects1[40];
	double Northing;
	double Easting;
	double Height;
	unsigned char Rejects2[48];
	unsigned char Checksum[4];

};

unsigned char ReceiveByte()
{
	unsigned char Ch = 0x11;
	//reading the serial port code must be here
	return Ch;
}

int main()
{
	unsigned char* BytePtr;
	unsigned char Byte;
	GPS GPSRecord;
	bool EndofPath = false;

	GPSRecord.FullHeader = 0x00;
	BytePtr = (unsigned char*)&GPSRecord;


	while (!EndofPath)
	{
		// Trapping the header
		while (1)
		{
			Byte = ReceiveByte(); //check the header we are receiving from the serial port
			GPSRecord.FullHeader <<= 8;
			GPSRecord.FullHeader |= Byte;
			if (GPSRecord.FullHeader == 0xAA44121C)
			{
				break; // Header found
			}
		}



		//filling in Entire structure in struct GPS, particularly northing, easting and height
		for (int i = 0; i < 108; i++)
			*(BytePtr + 44 + i) = ReceiveByte(); //least significant byte of double Northing i=o

		cout << GPSRecord.Easting << " " << GPSRecord.Northing << " " << GPSRecord.Height << endl;
	}
	return 0;
}*/