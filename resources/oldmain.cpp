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

unsigned char* getRandomColourPacket() {

	unsigned char* packet = new unsigned char[32];

	// {0xff,0x08,0x92,0x0a,0x02,0x20,0x02,0xff,0x20,0x53,0x39,0x00,0x02,0x2a,0x32,0x80,0x00,0x00,0x00,0x00,0x36,0xe7,0x3c,0x90,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*left light*/

	// random = random(0xFF);

	packet[0] = 0xFF; // header 1
	packet[1] = 0x08; // header 2
	packet[2] = 0x92; // header 3
	packet[3] = 0x0a; // inc
	packet[4] = 0x02; // panel
	packet[5] = 0x20; // unknown
	packet[6] = 0x02; // unknown

	packet[7] = 0x00; // r
	packet[8] = 0xff; // g
	packet[9] = 0x00; // b

	// packet[7] = (char)random();
	// packet[8] = (char)random();
	// packet[9] = (char)random();

	int checksum = 0;
	for(int l = 0 ; l < 10 ; l++) {
		checksum += packet[l];
	}

	checksum = checksum & 0xFF;

	packet[10] = checksum; // checksum
	packet[11] = 0x00; // unknown and seemingly useless from here on
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
	int retVal;

	// printf("Run %i: ",1);

	// for(int j = 0 ; j < 32 ; j++) {
	// 	printf("%2x ",packet[j]);
	// }
	// printf("\n");

	retVal = libusb_bulk_transfer(deviceHandler,0x01,packet,32,&len,0);

	if(retVal != 0) {
		printf("Error code: %d\n",retVal);
		printf("Error name: %s\n",libusb_error_name(retVal));
		exit(1);
	}

	// so that the device doesn't freeze up

	int retCount = 0;

	while(retVal == 0) {
		retVal = libusb_bulk_transfer(deviceHandler,0x81,packet,32,&len,10);

		if(retVal == 0) {

			// printf("%d: New len: %d\n",retCount,len);
			printf("Returned: ");

			for(int k = 0 ; k < 32 ; k++) {
				printf("%2x ",packet[k]);
			}

			printf("\n");
			retCount++;
		}
	}

}

void activateDevice(struct libusb_device_handle* deviceHandler) {

	unsigned char packet[] = {0xff,0x11,0x80,0x00,0x28,0x63,0x29,0x20,0x44,0x69,0x73,0x6e,0x65,0x79,0x20,0x32,0x30,0x31,0x33,0xb6,0x30,0x6f,0xcb,0x40,0x30,0x6a,0x44,0x20,0x30,0x5c,0x6f,0x00};
	sendPacket(deviceHandler, packet);
}

void fastChangeColour(struct libusb_device_handle* deviceHandler, char platform, char r, char g, char b) {

	// ff 06 90 41 02 00 00 00 d8 00 00 00 36 f1 2c 70 00 00 00 00 36 e7 3c 90 00 00 00 00 00 00 00 00

	unsigned char* packet = new unsigned char[32];

	packet[0] = 0xff;
	packet[1] = 0x06; // packet length
	packet[2] = 0x90; // weird packet length
	packet[3] = 0x41;
	packet[4] = platform; // platform
	packet[5] = r; // r
	packet[6] = g; // g
	packet[7] = b; // b
	// packet[8] = 0xd8;

	int checksum = 0;
	for(int l = 0 ; l < 8 ; l++) {
		checksum += packet[l];
	}

	checksum = checksum & 0xFF;

	packet[8] = checksum;
	packet[9] = 0x00;
	packet[10] = 0x00;
	packet[11] = 0x00;
	packet[12] = 0x36;
	packet[13] = 0xf1;
	packet[14] = 0x2c;
	packet[15] = 0x70;
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

	sendPacket(deviceHandler, packet);
}

void doRedFlash(struct libusb_device_handle* deviceHandler) {

	// ff 09 93 07 02 02 02 06 ff 00 00 ad 36 f1 2c 70 00 00 00 00 36 e7 3c 90 28 00 00 44 00 00 00 00

	unsigned char* packet = new unsigned char[32];

	packet[0] = 0xFF;
	packet[1] = 0x09; // packet length after this
	packet[2] = 0x93;
	packet[3] = 0x07;
	packet[4] = 0x01; // platform
	packet[5] = 0x02;
	packet[6] = 0x02;
	packet[7] = 0x06;
	packet[8] = 0xff; // r
	packet[9] = 0xff; // g
	packet[10] = 0x00; // b
	// packet[11] = 0xad;

	int checksum = 0;

	for(int l = 0 ; l < 11 ; l++) {
		checksum += packet[l];
	}

	packet[11] = checksum & 0xff;

	packet[12] = 0x36;
	packet[13] = 0xf1;
	packet[14] = 0x2c;
	packet[15] = 0x70;
	packet[16] = 0x00;
	packet[17] = 0x00;
	packet[18] = 0x00;
	packet[19] = 0x00;
	packet[20] = 0x36;
	packet[21] = 0xe7;
	packet[22] = 0x3c;
	packet[23] = 0x90;
	packet[24] = 0x28;
	packet[25] = 0x00;
	packet[26] = 0x00;
	packet[27] = 0x44;
	packet[28] = 0x00;
	packet[29] = 0x00;
	packet[30] = 0x00;
	packet[31] = 0x00;

	sendPacket(deviceHandler,packet);
}


void setColour(struct libusb_device_handle* deviceHandler, char platform, char r, char g, char b) {
	unsigned char* packet = new unsigned char[32];

	packet[0] = 0xFF; // header 1
	packet[1] = 0x08; // header 2
	packet[2] = 0x92; // header 3
	packet[3] = 0x0a; // inc
	packet[4] = platform; // panel
	packet[5] = 0x10; // unknown
	packet[6] = 0x02; // unknown

	packet[7] = r;
	packet[8] = g;
	packet[9] = b;

	int checksum = 0;
	for(int l = 0 ; l < 10 ; l++) {
		checksum += packet[l];
	}

	checksum = checksum & 0xFF;

	packet[10] = checksum; // checksum
	packet[11] = 0x00; // unknown and seemingly useless from here on
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

	sendPacket(deviceHandler,packet);
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

	// InfinityPortal infinityPortal;

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
	int packetCount = 1;
	// int packetCount = 3;

	string inputString;

	libusb_bulk_transfer(deviceHandler,0x81,packets[1],32,&len,100);

	// for(int i = 0 ; i < packetCount ; i++) {
		
	// 	printf("Run %i: ",i+1);

	// 	for(int j = 0 ; j < 32 ; j++) {
	// 		printf("%2x ",packets[i][j]);
	// 	}
	// 	printf("\n");

	// 	retVal = libusb_bulk_transfer(deviceHandler,0x01,packets[i],32,&len,0);

	// 	if(retVal != 0) {
	// 		printf("Error code: %d\n",retVal);
	// 		printf("Error name: %s\n",libusb_error_name(retVal));
	// 		return -1;
	// 	}

	// 	// so that the device doesn't freeze up

	// 	while(retVal == 0) {
	// 		retVal = libusb_bulk_transfer(deviceHandler,0x81,packets[i],32,&len,100);

	// 		if(retVal == 0) {
	// 			printf("Returned: ");

	// 			for(int k = 0 ; k < 32 ; k++) {
	// 				printf("%2x ",packets[i][k]);
	// 			}

	// 			printf("\n");
	// 		}
	// 	}

	// 	// getline(cin, inputString);

	// 	usleep(500000);
	// }

	activateDevice(deviceHandler);

	// unsigned char packet[32];

	printf("Starting random packet test!\n");

	int swap = 0;

	while(true) {

		// packet = getRandomColourPacket();

		// sendPacket(deviceHandler,getRandomColourPacket());

		// printf("Swap: %d %d %d\n",((swap+1)%3),((swap+2)%3),((swap+3)%3));

		// fastChangeColour(deviceHandler,((swap+1)%3)+1,0xFF,0x00,0x00);
		// fastChangeColour(deviceHandler,((swap+2)%3)+1,0x00,0xff,0x00);
		// fastChangeColour(deviceHandler,((swap+3)%3)+1,0x00,0x00,0xff);

		// swap = swap+4;

		fastChangeColour(deviceHandler,random()%3+1,random()%0x100,random()%0x100,random()%0x100);

		// doRedFlash(deviceHandler);

		// fastChangeColour(deviceHandler);

		usleep(100000);
		// setColour(deviceHandler,0x00,0x00,0x00,0x00);
		// usleep(1500000);
	}
	
	return 0;
}

