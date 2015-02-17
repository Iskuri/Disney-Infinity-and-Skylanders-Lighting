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

using namespace std;

unsigned char* getRandomColourPacket() {

	unsigned char packet[32];

	// {0xff,0x08,0x92,0x0a,0x02,0x20,0x02,0xff,0x20,0x53,0x39,0x00,0x02,0x2a,0x32,0x80,0x00,0x00,0x00,0x00,0x36,0xe7,0x3c,0x90,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*left light*/

	// random = random(0xFF);

	packet[0] = 0xFF;
	packet[1] = 0x08;
	packet[2] = 0x92;
	packet[3] = 0x0a;
	packet[4] = 0x02; // panel
	packet[5] = 0x20; // unknown
	packet[6] = 0x02; // unknown
	packet[7] = 0xff; // unknown
	packet[8] = 0x20; // unknown
	packet[9] = 0x53; // unknown
	packet[10] = 0x39; //// checksum!!
	packet[11] = 0x00;
	packet[12] = 0x02;
	packet[13] = 0x2a;
	packet[14] = 0x32;
	packet[15] = 0x80;
	packet[16] = 0x00;
	packet[17] = 0x00;
	packet[18] = 0x00;
	packet[19] = 0x00;
	packet[20] = 0x36;
	packet[21] = 0xe7;
	packet[22] = 0x3c;
	packet[23] = 0x90;
	packet[24] = 0x00;
	packet[25] = 0x00;
	packet[26] = 0x00;
	packet[27] = 0x00;
	packet[28] = 0x00;
	packet[29] = 0x00;
	packet[30] = 0x00;
	packet[31] = 0x00;

	return packet;
}

void sendPacket(struct libusb_device_handle* deviceHandler, unsigned char* packet) {

	int len;

	retVal = libusb_bulk_transfer(deviceHandler,0x01,packets[i],32,&len,0);

}

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

	int error;

	struct libusb_device_descriptor descriptor;
	libusb_device_handle* deviceHandler;

	int retVal = 0;

	for(int i = 0 ; i < devicesCount ; i++) {

			retVal = libusb_open(devices[i], &tryDeviceHandler);

			if(retVal < 0) {
					continue;
			}

			libusb_get_device_descriptor(devices[i], &descriptor);

//              if(descriptor.idVendor == 0x1430 && descriptor.idProduct == 0x150) {
			if(descriptor.idVendor == 0x0e6f && descriptor.idProduct == 0x0129) {

					// printf("Got it!\n");

					deviceHandler = tryDeviceHandler;
					break;
			}
	}

	if (libusb_kernel_driver_active(deviceHandler, 0) == 1) {
		retVal = libusb_detach_kernel_driver(deviceHandler, 0);
		if (retVal < 0) {
				libusb_close(deviceHandler);
		}
	}

	retVal = libusb_claim_interface(deviceHandler, 0);

	int len = 0;

	// ff 08 92 49 02 24 02 ff 5a 00 63 00 36 f1 2c 70 00 00 00 00 36 e7 3c 90 28 00 00 44 00 00 00 00

	unsigned char packets[][32] = {
		{0xff,0x11,0x80,0x00,0x28,0x63,0x29,0x20,0x44,0x69,0x73,0x6e,0x65,0x79,0x20,0x32,0x30,0x31,0x33,0xb6,0x30,0x6f,0xcb,0x40,0x30,0x6a,0x44,0x20,0x30,0x5c,0x6f,0x00},/*active*/
	//  {0xff,0x08,0x92,0x09,0x02,0x51,0x01,0xfc,0xbd,0x7e,0x2d,0x00,0x02,0x2a,0x32,0x80,0x00,0x00,0x00,0x00,0x36,0xe7,0x3c,0x90,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*left light*/
		{0xff,0x08,0x92,0x0a,0x02,0x20,0x02,0xff,0x20,0x53,0x39,0x00,0x02,0x2a,0x32,0x80,0x00,0x00,0x00,0x00,0x36,0xe7,0x3c,0x90,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*left light*/
		{0xff,0x08,0x92,0x0a,0x01,0x51,0x01,0xfc,0xbd,0x7e,0x2d,0x00,0x02,0x2a,0x32,0x80,0x00,0x00,0x00,0x00,0x36,0xe7,0x3c,0x90,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*top light*/
		{0xff,0x08,0x92,0x43,0x03,0x51,0x01,0xfc,0xbd,0x7e,0x68,0x00,0x36,0xf1,0x2c,0x70,0x00,0x00,0x00,0x00,0x36,0xe7,0x3c,0x90,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*right light*/
		{0xff,0x08,0x92,0x17,0x02,0x51,0x01,0x00,0x00,0x00,0x04,0x00,0x02,0x2a,0x32,0x80,0x00,0x00,0x00,0x00,0x36,0xe7,0x3c,0x90,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
		{0xff,0x08,0x92,0x18,0x01,0x51,0x01,0x00,0x00,0x00,0x04,0x00,0x02,0x2a,0x32,0x80,0x00,0x00,0x00,0x00,0x36,0xe7,0x3c,0x90,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
		{0xff,0x08,0x92,0x46,0x03,0x51,0x01,0x00,0x00,0x00,0x34,0x00,0x36,0xf1,0x2c,0x70,0x00,0x00,0x00,0x00,0x36,0xe7,0x3c,0x90,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	};
	int packetCount = 2;
	// int packetCount = 3;

	string inputString;

	libusb_bulk_transfer(deviceHandler,0x81,packets[1],32,&len,100);

	for(int i = 0 ; i < packetCount ; i++) {
		
		printf("Run %i: ",i+1);

		for(int j = 0 ; j < 32 ; j++) {
			printf("%2x ",packets[i][j]);
		}
		printf("\n");

		retVal = libusb_bulk_transfer(deviceHandler,0x01,packets[i],32,&len,0);

		if(retVal != 0) {
			printf("Error code: %d\n",retVal);
			printf("Error name: %s\n",libusb_error_name(retVal));
			return -1;
		}

		// so that the device doesn't freeze up

		while(retVal == 0) {
			retVal = libusb_bulk_transfer(deviceHandler,0x81,packets[i],32,&len,100);

			if(retVal == 0) {
				printf("Returned: ");

				for(int k = 0 ; k < 32 ; k++) {
					printf("%2x ",packets[i][k]);
				}

				printf("\n");
			}
		}

		// getline(cin, inputString);

		usleep(500000);
	}
	
	return 0;
}
