#ifndef INFINITYPORTAL_H
#define	INFINITYPORTAL_H

#include <stdio.h>
#include "libusb-1.0/libusb.h"
#include <string.h>
#include <stdlib.h> 
#include <stdio.h>
#include <sys/types.h>
#include <iostream>
#include <cstdlib>
#include <math.h>
#include <unistd.h>

class InfinityPortal {
public:
	InfinityPortal(int deviceId);
	virtual ~InfinityPortal();
	void writeData(char* data, int dataLength);
	libusb_device_handle* deviceHandler;
	void setColour(char platform, char r, char g, char b);
	void flashColour(char platform, char r, char g, char b);
	void fadeColour(char platform, char r, char g, char b);
	void activate();
private:
	void processReceivedPacket(unsigned char* packet);
	void sendPacket(unsigned char* packet);
	int receivePackets();
	libusb_device_handle* connect(int deviceId);
};

#endif