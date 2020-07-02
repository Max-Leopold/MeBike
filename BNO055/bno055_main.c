/*
 * bno055_main.c
 *
 * Created: 08.06.2020 22:06:48
 *  Author: nicok
 */ 

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include "bno055_main.h"
#include "../uart/serial.h"
#include "../i2cmaster/i2cmaster.h"
#include "../util/util.h"
#include "stdio.h"
#include <util/delay.h>
#include <avr/io.h>
#include <math.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#define DEVICE_ADDRESS (0x28 << 1)

struct BNODATA bnodata;

bno_init(){

    i2c_init();									   // init I2C interface
	_delay_ms(1000);							   // important, so the sensor has enough time to boot properly, before setting the operation mode 
	
    /* write 0x50 As device address and add as 8th bit a 0 to write or 1 to read */
	//Set NDOF mode
    i2c_start_wait(DEVICE_ADDRESS+I2C_WRITE);       // Set device address and write mode
	i2c_write(0x3D);								// Mode Selection Register
	i2c_write(0x0C);								// Set NDOF mode to use accelerometer and gyroscope
	i2c_stop();
}

void bno055_main(){
readPitch();
}

//Method to read and print single register of 8 bit over uart
//param: Address of the register to read
void printRegister(uint8_t address){
	i2c_start_wait(DEVICE_ADDRESS+I2C_WRITE);			// send start condition Sensor Address with R/W Bit (LSB, Bit 8) = 0
	i2c_write(address);									// write the register address
	i2c_start(DEVICE_ADDRESS+I2C_READ);					// send start condition Sensor Address with R/W Bit (LSB, Bit 8) = 1
	uint8_t rtn = i2c_readNak();						// read the previously set register and send NACK, so the sensor won't read the subsequent register
	i2c_stop();											// stop the i2c communication

	char str[4];
	convertIntToString(rtn, 4, &str);
	serial_print_line(str);
}

//Method to write value to specific register
//param address: register address
//param value: value to write 
void writeRegister(uint8_t address, uint8_t value){
	i2c_start(DEVICE_ADDRESS+I2C_WRITE);				// send start condition Sensor Address with R/W Bit (LSB, Bit 8) = 0
	i2c_write(address);									// write the register address
	i2c_write(value);									// write the value in the register
	i2c_stop();											// stop the i2c communication
}												   

//Method to read single register of 8 bit
//param: Address of the register to read
uint8_t readRegister(uint8_t reg){						
	i2c_start(DEVICE_ADDRESS+I2C_WRITE);		   		// send start condition Sensor Address with R/W Bit (LSB, Bit 8) = 0
	i2c_write(reg);										// write the register address
	i2c_start(DEVICE_ADDRESS+I2C_READ);					// send start condition Sensor Address with R/W Bit (LSB, Bit 8) = 1
	uint8_t returnVal = i2c_readNak();					// read the previously set register and send NACK, so the sensor won't read the subsequent register
	i2c_stop();											// stop the i2c communication
	return returnVal;
}

//Method to read the current pitch data of the BNO055 and store it in the BNODATA struct
void readPitch(){
	int pitch;
	
	i2c_start(DEVICE_ADDRESS+I2C_WRITE);				// send start condition Sensor Address with R/W Bit (LSB, Bit 8) = 0
	i2c_write(0x1E);									// write the register address. 0x1E as LSB address of Pitch data Register
	i2c_start(DEVICE_ADDRESS+I2C_READ);					// send start condition Sensor Address with R/W Bit (LSB, Bit 8) = 1
														
	pitch =  i2c_readAck();								// read register and send ACK. Sensor will automatically send subsequent register 0x1F. Return value is LSB of pitch data and will be saved at bits 0 to 7
	pitch |= i2c_readNak()<<8;							// read register and send NACK, so the sensor won't read the subsequent register. Shift result by 8 bits, so the MSB part is at bits 8 to 15
	i2c_stop();											// stop the i2c communication

	pitch = (pitch/ 16.0);
	
	char str[4];
	// TODO: change to own method, but not yet working 
	//convertIntToString(bnogyro.Pitch, 4, &str);
	sprintf(str, "%d", pitch);
	bnodata.Pitch = str;
	serial_print_line(str);
	return bnodata.Pitch;
}




