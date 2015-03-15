#include "InfinityPortal.h"

InfinityPortal::InfinityPortal(int deviceId) {
	
	// printf("Device id: %d\n",deviceId);
	deviceHandler = connect(deviceId);
	
	int retVal = 0;
	
	if (libusb_kernel_driver_active(deviceHandler, 0) == 1) {
		retVal = libusb_detach_kernel_driver(deviceHandler, 0);
		if (retVal < 0) {
			libusb_close(deviceHandler);
		}
	}

	retVal = libusb_claim_interface(deviceHandler, 0);

	if(retVal != 0) {
		printf("Error code: %d\n",retVal);
		printf("Error name: %s\n",libusb_error_name(retVal));
		exit(1);
	}

	activate();
}

InfinityPortal::InfinityPortal() {

}

libusb_device_handle* InfinityPortal::connect(int deviceId) {
	
	libusb_device** devices;
	libusb_context* context;
	struct libusb_device_handle* tryDeviceHandler;

	libusb_init(&context);
	int devicesCount = libusb_get_device_list(context, &devices);
	
	int error;

	struct libusb_device_descriptor descriptor;
	
	int retVal = libusb_open(devices[deviceId], &tryDeviceHandler);
	
	libusb_get_device_descriptor(devices[deviceId], &descriptor);

	if(descriptor.idVendor == 0x0e6f && descriptor.idProduct == 0x0129) {

		return tryDeviceHandler;
	}
}

void InfinityPortal::getTagId() {

	// ff 03 b4 26 00 dc 02 06 ff 00 00 ca 36 f1 2c 70 00 00 00 00 36 e7 3c 90 00 00 00 00 00 00 00 00
	unsigned char* packet = new unsigned char[32];

	packet[0] = 0xff;
	packet[1] = 0x03;
	packet[2] = 0xb4;
	packet[3] = 0x26;
	packet[4] = 0x00;
	packet[5] = 0xdc;
	packet[6] = 0x02;
	packet[7] = 0x06;
	packet[8] = 0xff;
	packet[9] = 0x00;
	packet[10] = 0x00;
	packet[11] = 0xca;
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
	packet[25] = 0x00;
	packet[26] = 0x00;
	packet[27] = 0x00;
	packet[28] = 0x00;
	packet[29] = 0x00;
	packet[30] = 0x00;
	packet[31] = 0x00;

	sendPacket(packet);
}

void InfinityPortal::sendPacket(unsigned char* packet) {

	int len;
	int retVal = -1;

	receivePackets();

	while(retVal < 0) {
		retVal = libusb_bulk_transfer(deviceHandler,0x01,packet,32,&len,100);
		receivePackets();
	}

	// if(retVal != 0) {
	// 	printf("Error code: %d\n",retVal);
	// 	printf("Error name: %s\n",libusb_error_name(retVal));
	// 	exit(1);
	// }
	
}

void InfinityPortal::processReceivedPacket(unsigned char* packet) {

	if(packet[0x00] == 0xab) {
		// printf("Something was placed somewhere!\n");

		// printf("Received: ");

		// for(int i = 0 ; i < 32 ; i++) {
		// 	printf("%x ",packet[i]);
		// }
		// printf("\n");

		char platformSetting = packet[2];
		char placedRemoved = packet[5];

		if(placedRemoved == 0x00) {
			printf("Tag placed on platform: %d\n",platformSetting);
		} else {
			printf("Tag removed from platform: %d\n",platformSetting);
		}

		getTagId();

	} else if(packet[0x00] == 0xaa && packet[0x01] == 0x09) {
		printf("Got tag info\n");

		// make print tag info!!!
		for(int i = 10 ; i > 2 ; i--) {
			printf("%x ",packet[i]);
		}
		printf("\n");
	}

}

int InfinityPortal::receivePackets() {

	int packetsReceived;
	int retVal = 0;
	int len = 0;
	unsigned char* packet = new unsigned char[32];

	while(retVal == 0) {

		retVal = libusb_bulk_transfer(deviceHandler,0x81,packet,32,&len,10);
		if(retVal == 0) {
			processReceivedPacket(packet);
			packetsReceived += 1;
		}
	}

	return packetsReceived;
}

void InfinityPortal::fadeColour(char platform, char r, char g, char b) {
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

	sendPacket(packet);
}

InfinityPortal::~InfinityPortal() {
	
}

void InfinityPortal::activate() {
	unsigned char packet[] = {0xff,0x11,0x80,0x00,0x28,0x63,0x29,0x20,0x44,0x69,0x73,0x6e,0x65,0x79,0x20,0x32,0x30,0x31,0x33,0xb6,0x30,0x6f,0xcb,0x40,0x30,0x6a,0x44,0x20,0x30,0x5c,0x6f,0x00};
	sendPacket(packet);
}

void InfinityPortal::setColour(char platform, char r, char g, char b) {

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

	sendPacket(packet);
}

void InfinityPortal::flashColour(char platform, char r, char g, char b) {

	// ff 09 93 07 02 02 02 06 ff 00 00 ad 36 f1 2c 70 00 00 00 00 36 e7 3c 90 28 00 00 44 00 00 00 00

	unsigned char* packet = new unsigned char[32];

	packet[0] = 0xFF;
	packet[1] = 0x09; // packet length after this
	packet[2] = 0x93;
	packet[3] = 0x07;
	packet[4] = platform;
	packet[5] = 0x02;
	packet[6] = 0x02;
	packet[7] = 0x06;
	packet[8] = r; // r
	packet[9] = g; // g
	packet[10] = b; // b

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

	sendPacket(packet);
}

