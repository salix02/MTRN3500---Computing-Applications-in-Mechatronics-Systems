#ifndef GNSS_H
#define GNSS_H

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif // !WIN32_LEAN_AND_MEAN

#include <EthernetClient.h>

#pragma pack(1)

struct GPSNova
{
	// Header bytes 0xAA, 0x44, 0x12, 0x1C (number of header bytes)
	unsigned int FullHeader;	//4
	unsigned char Rejects1[40]; //44	= 48
	double Northing;			//8		= 56
	double Easting;				//8		= 64
	double Height;				//8		= 72
	unsigned char Rejects2[40]; //40	= 112
	unsigned int Checksum;		//4		= 116
}; 

class GNSS : public EthernetClient
{
private:
	GPSNova GPSRec;
	bool EndOfPath;
	unsigned char* BytePtr;
	unsigned char Byte;
public:
	GNSS();
	GNSS(char * portaddress, char * port);
	~GNSS();
	void GetData();
	unsigned char ReceiveByte(int i);
	double getNorthing();
	double getEasting();
	double getHeight();

	//checksum checking
	unsigned long CRC32Value(int i);
	unsigned long CalculateBlockCRC32(unsigned long ulCount, unsigned char *ucBuffer); /* Number of bytes in the data block */ /* Data block */
};

#endif
 