/* 
 * File:   main.cpp
 * Author: christopher
 *
 * Created on 10 August 2014, 21:09
 */

#include <cstdlib>
#include <stdio.h>
#include <sys/types.h>
#include <iostream>
#include <math.h>
#include "libusb-1.0/libusb.h"
#include <unistd.h>
#include "InfinityPortal.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {

	srand (time(NULL));

	libusb_device** devices;
	libusb_context* context;
	struct libusb_device_handle* tryDeviceHandler;

	libusb_init(&context);
	int devicesCount = libusb_get_device_list(context, &devices);

	struct libusb_device_descriptor descriptor;
	libusb_device_handle* deviceHandler;

	int retVal = 0;

	for(int i = 0 ; i < devicesCount ; i++) {

			retVal = libusb_open(devices[i], &tryDeviceHandler);

			if(retVal < 0) {
					continue;
			}

			libusb_get_device_descriptor(devices[i], &descriptor);

			if(descriptor.idVendor == 0x0e6f && descriptor.idProduct == 0x0129) {

					InfinityPortal infinityPortal(i);

					while(true) {
						// infinityPortal.setColour(0,0xFF,0x00,0x00);
						infinityPortal.setColour(random()%3+1,random()%0x100,random()%0x100,random()%0x100);
						usleep(100000);
					}

			}
	}

	printf("Got out of the loop!\n");
	// while(true) {

	// 	fastChangeColour(deviceHandler,random()%3+1,random()%0x100,random()%0x100,random()%0x100);

	// 	usleep(100000);
	// }
	
	return 0;
}
