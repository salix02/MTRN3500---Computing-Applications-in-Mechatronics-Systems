#ifndef SMLASER_H
#define SMLASER_H

#include <Windows.h>
#include <tchar.h>
#include "SMfcn.h"

class SMObject
{
	HANDLE CreateHandle;
	HANDLE AccessHandle;
	TCHAR *szName;
	int Size;
public:
	void *pData;		//pointer to first address of the memory chunk that you ask for, have to typecast
	int SMCreateError;	//flags
	int SMAccessError;
public:
	SMObject();
	SMObject(TCHAR* szname, int size); //Tag Name to be shared by all program, size 

	~SMObject();
	int SMCreate();		//create the memory
	int SMAccess();		//access the memory, read or write
};
#endif


