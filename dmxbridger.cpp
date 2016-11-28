/*
 * dmxbridger.cpp
 *
 *  Created on: 27.11.2016
 *      Author: oliver
 */

#include <iostream>
#include <stdio.h>
#include <ftdi.h>
#include <unistd.h>

using namespace std;

class Dmxbridge {
public:
	Dmxbridge();
	int open();
	int send(unsigned char data[]);
	int find_all();
private:
	struct ftdi_context *pFtdi;
	unsigned char buffer[512];
	unsigned char reset[1];
};

Dmxbridge::Dmxbridge(void) {
	this->reset[0] = 0x00;

	if ((this->pFtdi = ftdi_new()) == false) {
		fprintf(stderr, "ftdi_new context FAILURE\n");
	} else {
		fprintf(stderr, "ftdi_new context SUCCESS\n");
	}
}

int Dmxbridge::open() {
	cout << "Try to open usb ... \n";
	int ret = EXIT_SUCCESS;

	ret = ftdi_usb_open(this->pFtdi, 0x0403, 0x6001);

	if(ret < 0) {
		fprintf(stderr, "USB open FAILURE: %s (%s)\n", ret, ftdi_get_error_string(this->pFtdi));
	} else {
		fprintf(stderr, "USB open SUCCESS: %s (%s)\n", ret, ftdi_get_error_string(this->pFtdi));
	}

	ftdi_set_line_property(this->pFtdi, BITS_8, STOP_BIT_2, NONE);

	return ret;
}

int Dmxbridge::send(unsigned char data[]) {
	this->buffer[2] = 0xFF;
	this->buffer[9] = 0xFF;
	this->buffer[11] = 0xFF;

	//Reset line
	ftdi_set_baudrate(this->pFtdi, 96000);
	ftdi_transfer_data_done(ftdi_write_data_submit(this->pFtdi, this->reset, 1));

	//Send data
	ftdi_set_baudrate(this->pFtdi, 250000);
	ftdi_transfer_data_done(ftdi_write_data_submit(this->pFtdi, data, 512));

	return 0;
}

int Dmxbridge::find_all() {

}

int main() {
	int ret = EXIT_SUCCESS;

	Dmxbridge * db = new Dmxbridge;

	if((ret = db->open()) < 0) {
		delete db;
		return ret;
	}

	unsigned char data[512];

	data[0] = 0xFF;
	data[9] = 0x00;
	data[11] = 0xFF;

	for(int i = 0; i < 255; i++) {
		data[9] = i;
		data[11] = 0xFF - i;

		if(( ret = db->send(data)) < 0) {
			delete db;
			return ret;
		}

		usleep(25000);
	}

	delete db;

	cout << "Finished! :-)\n";

	return ret;
}
