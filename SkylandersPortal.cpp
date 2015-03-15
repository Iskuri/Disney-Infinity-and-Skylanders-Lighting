#include "SkylandersPortal.h"

SkylandersPortal::SkylandersPortal(int deviceId) {

	deviceHandler = connect(deviceId);

	int retVal;

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

}

SkylandersPortal::SkylandersPortal() {

}

SkylandersPortal::~SkylandersPortal() {
	
}

libusb_device_handle* SkylandersPortal::connect(int deviceId) {
	
	libusb_device** devices;
	libusb_context* context;
	struct libusb_device_handle* tryDeviceHandler;

	libusb_init(&context);
	int devicesCount = libusb_get_device_list(context, &devices);
	
	int error;

	struct libusb_device_descriptor descriptor;
	
	int retVal = libusb_open(devices[deviceId], &tryDeviceHandler);
	
	libusb_get_device_descriptor(devices[deviceId], &descriptor);

	if(descriptor.idVendor == 0x1430 && descriptor.idProduct == 0x150) {

		return tryDeviceHandler;
	}
}

void SkylandersPortal::setColour(char r, char g, char b) {
	
	char* data = new char[32];
	data[0] = 'C';
	data[1] = r;
	data[2] = g;
	data[3] = b;
	writeData(data);
}

void SkylandersPortal::setLeftColour(char r, char g, char b) {
	char data[] = {0x4a, 0x02, r, g, b, 0x00, 0x00};
	// int dataLength = 7;
	writeData((char*)data);
}

void SkylandersPortal::setRightColour(char r, char g, char b) {
	char data[] = { 0x4a, 0x00, r, g, b, 0x00, 0x00};
	// int dataLength = 7;
	writeData((char*)data);
}

unsigned char* SkylandersPortal::readData(unsigned char* data) {

	int retVal = -7;
	int transferred;
	
	// while(retVal == -7) {
		retVal = libusb_interrupt_transfer(deviceHandler, 129, data, 32, &transferred, 100);
	// }

	// printf("Retval in skylander portal: %d\n",retVal);
	
	return data;
}

unsigned char SkylandersPortal::getVersion() {

}

void SkylandersPortal::writeData(char* data) {

	int retVal = -1;
	int reportNumber = data[0];
	int failures = 0;

	while(retVal < 0) {

		// printf("Values: %x %x %x\n",LIBUSB_REQUEST_TYPE_CLASS|LIBUSB_RECIPIENT_INTERFACE|LIBUSB_ENDPOINT_OUT,
		// 		0x09,
		// 		(2 << 8) | reportNumber);
		// exit(1);

		retVal = libusb_control_transfer(deviceHandler,
				LIBUSB_REQUEST_TYPE_CLASS|LIBUSB_RECIPIENT_INTERFACE|LIBUSB_ENDPOINT_OUT,
				0x09,
				(2 << 8) | reportNumber,
				0,
				(unsigned char *)data, 32,
				50);

		failures++;

		// printf("Failed %d\n",failures);

		if(failures > 100) {
			break;
		}
	}

	if(retVal < 0) {
		// printf("Returned val %d\n",retVal);
		// printf("Got error %s\n",libusb_error_name(retVal));
		// exit(1);
	}
}

void SkylandersPortal::activateSpeaker() {

	char data[] = {0x4d,0x01};
	int dataLength = 2;
	writeData(data);
}

void SkylandersPortal::reset() {
	char* data = new char[32];
	data[0] = 'R';
	writeData(data);	
}

void SkylandersPortal::set() {
	char* data = new char[32];
	data[0] = 'A';
	data[1] = 0x01;
	writeData(data);	
}

void SkylandersPortal::flashTrapLight() {

	char* data = new char[32];
	data[0] = 0x51;
	data[1] = 0x10;
	data[2] = 0x08;

	// i hope this does something
	writeData(data);
}

void SkylandersPortal::writeMaskerMind() {

	unsigned char packets[][32] = {
		{0x57,0x12,0x09,0x45,0xe0,0x90,0xe6,0x45,0x6c,0xf4,0x73,0xbe,0xb8,0x22,0xe6,0x0d,0xdb,0xdf,0x2a},
		{0x57,0x12,0x0d,0x3f,0x84,0xe9,0x22,0xcd,0x2d,0x49,0xa8,0xb4,0xe0,0xac,0x83,0x54,0xb7,0xcd,0x77},
		{0x57,0x12,0x11,0x86,0xb6,0x7f,0x43,0xf7,0xa6,0x83,0x4c,0x8b,0xf5,0x90,0xc6,0x18,0xda,0x29,0xcd},
		{0x57,0x12,0x21,0xe3,0x2b,0xf2,0xdc,0x6f,0xaf,0xb5,0x7f,0x9c,0x67,0x10,0x0e,0x83,0x2e,0x9c,0x10},
		{0x57,0x12,0x08,0x62,0x3d,0xee,0xc6,0x05,0xc7,0x0d,0xd9,0xee,0x91,0x17,0xf3,0x30,0x6a,0x91,0x78},
		{0x57,0x10,0x0d,0x5a,0xff,0x62,0x7f,0x10,0x12,0x6b,0x0d,0x81,0x9f,0x41,0x90,0x44,0xce,0xd3,0x6c},
		{0x57,0x10,0x10,0x4f,0xfa,0xa5,0x64,0x81,0x78,0xb9,0xf8,0xb3,0xf6,0x82,0xeb,0xe5,0x6a,0x45,0x5d},
		{0x57,0x10,0x08,0x31,0x23,0x26,0xbe,0x33,0xa3,0xa7,0xd2,0x2c,0x1a,0x67,0xbb,0xf5,0x79,0x4b,0x56},
		{0x57,0x11,0x29,0x7d,0x08,0xc5,0x37,0x49,0xdc,0x00,0xbf,0xaa,0x4f,0x8a,0xda,0x3c,0xe4,0xc8,0x07},
		{0x57,0x11,0x2c,0xa4,0xf9,0xe2,0x66,0x0f,0xbc,0xbf,0x37,0x36,0x8f,0xac,0x82,0x07,0x9a,0x4a,0xdb},
		{0x57,0x11,0x24,0xe7,0xd7,0x03,0xea,0xcd,0x92,0x06,0x57,0x7d,0x57,0x23,0x47,0x28,0x43,0x0a,0x9c},
	};

	int packetCount = 12;

	for(int i = 0 ; i < packetCount ; i++) {
		// writeData((char*)packets[i]);

		// this needs the report number sorting out then it should be good to go
		int retVal = libusb_control_transfer(deviceHandler,
				LIBUSB_REQUEST_TYPE_CLASS|LIBUSB_RECIPIENT_INTERFACE|LIBUSB_ENDPOINT_OUT,
				0x09,
				// (2 << 8) | reportNumber,
				0x243, // THIS SHOULD BE RIGHT!!
				0,
				packets[i], 19,
				50);
		printf("Wrote packet: %d\n",i);
		usleep(100000);
	}

}

void SkylandersPortal::tryAudio() {

	unsigned char* data = new unsigned char[32];

	FILE* audioFile = fopen("resources/audiodatabin","r");
	int len = 0;
	int retVal = 0;

	while(!feof(audioFile)) {

		fgets((char*)data,32,audioFile);

		printf("Running audio: ");

		for(int i = 0 ; i < 32 ; i++) {
			printf("%x ",data[i]);
		}
		printf("\n");
		
		// this system uses bulk transfers for audio, and as I am yet to find anywhere else where bulk transfers are used, this can be left here
		retVal = libusb_bulk_transfer(deviceHandler,0x02,data,32,&len,0);

	}
}

unsigned char* SkylandersPortal::getFigures() {
	
	char* data = new char[32];
	unsigned char* figureData = new unsigned char[32];
//	unsigned char* returnFigureData = new unsigned char[0x21];

	data[0] = 'Q';
	data[1] = 0x10;
	data[2] = 0x01;
	
	writeData(data);

	figureData = readData(figureData);

	// printf("Returned values: %c, %x, %x\n",figureData[0],figureData[1],figureData[2]);

	return figureData;
	
}