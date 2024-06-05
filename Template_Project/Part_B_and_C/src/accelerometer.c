#include "SPI.h"
#include "SysTimer.h"
#include "accelerometer.h"

int volatile test;

void accWrite(uint8_t addr, uint8_t val){
	// TODO access SPI_Transfer_Data
	uint16_t write_data = (uint16_t)addr << 8 | val;
	SPI_Transfer_Data(write_data);
}

uint8_t accRead(uint8_t addr){
	// access SPI_Transfer_Data
		uint16_t write_data = 0;
		write_data = (uint16_t)addr << 8 | 0x8000;
		uint16_t read_data = SPI_Transfer_Data(write_data);
		
		return read_data & 0xFF;
}

void initAcc(void){
	
	test = accRead(0x00);
	
	// set 100Hz output
	
	accWrite(0x2C, 0x0A);
	
	// set full range mode

	accWrite(0x31, 0x0B);

	// enable measurement
	
	accWrite(0x2D, 0x08);
	
}

void readValues(double* x, double* y, double* z){
	// TODO
	// find scaler from data sheet
	// read values into x,y,z using accRead
	*x = (int16_t)((int16_t)accRead(0x33) << 8 | accRead(0x32));
	
	*y = (int16_t)((int16_t)accRead(0x35) << 8 | accRead(0x34));
	
	*z = (int16_t)((int16_t)accRead(0x37) << 8 | accRead(0x36));
}
