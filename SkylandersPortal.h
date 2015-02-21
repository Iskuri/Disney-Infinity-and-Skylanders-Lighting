#ifndef SKYLANDERSPORTAL_H
#define	SKYLANDERSPORTAL_H

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
// #include "PortalAudio.h"

class SkylandersPortal {
public:
	SkylandersPortal(int deviceId);
	SkylandersPortal();
	virtual ~SkylandersPortal();
	void setColour(char r, char g, char b);
	void setLeftColour(char r, char g, char b);
	void setRightColour(char r, char g, char b);
	unsigned char* getFigures();
	unsigned char getVersion();
	void activateSpeaker();
	void tryAudio();
	void reset();
	void set();
	void flashTrapLight();
private:
	libusb_device_handle* deviceHandler;
	void writeData(char* data);
	unsigned char* readData(unsigned char* data);
	libusb_device_handle* connect(int deviceId);
};

#endif