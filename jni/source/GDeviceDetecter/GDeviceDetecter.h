#ifndef GDeviceDetecter_h
#define GDeviceDetecter_h

#include <iostream>
#include <stdint.h>

using namespace std;

class GDeviceDetecter
{
public:
	void detect(void *callback, void *data);
	friend void GDeviceDetecter_RunLoop_C(void *data);

private:
	void runLoop();
	int  createSocket();

private:
	void *mCallback;
	void *mData;

/////////////////////////////////////////////////////////////////////////////////////////////////
//
//get local IPAddress
//
//add by jarry.jin 2014-04-02
//
/////////////////////////////////////////////////////////////////////////////////////////////////
public:
	void parseLocalIPAddress(const char *ip);
	int  localIPAddress(int index);

private:
	void parseLocalIPAddressList(char list[][100], int *num, int max);
	void parseIPAddress3Number(const char *ip, char *cip, int maxsize);
	void convertIPV4Address2IntArray(const char *ip, int *iip);

private:
	int   mLocalIPAddress[4];
};

#endif
