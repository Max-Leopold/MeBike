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


#define DEVICE_ADDRESS 0x50
struct BNOGYRO bnogyro;

bno_init(){
	
	unsigned char ret;
    
    DDRB  = 0xff;                              // use all pins on port B for output 
    PORTB = 0xff;                              // (active low LED's )

    i2c_init();                                // init I2C interface
	_delay_ms(1000);							   // important, so the sensor has enough time to boot properly, before setting the operation mode
//working
    /* write 0x50 As device address and add as 8th bit a 0 to write or 1 to read */
	//Set ACCGYRO mode
    i2c_start_wait(DEVICE_ADDRESS+I2C_WRITE);       // Set device address and write mode
	i2c_write(0x3D);								// Mode Selection Register
	i2c_write(0x0C);								// Set NDOF mode to use accelerometer and gyroscope
	i2c_stop();
}

void bno055_main(){
readPitch();
}

void printRegister(uint8_t address){
	i2c_start_wait(DEVICE_ADDRESS+I2C_WRITE);		   // set device address and write mode
	i2c_write(address);
	i2c_start(DEVICE_ADDRESS+I2C_READ);
	uint8_t rtn = i2c_readNak();
	i2c_stop();

	char str[4];
	convertIntToString(rtn,4, &str);
	serial_print_line( str);
}

void writeRegister(uint8_t address, uint8_t value){
	i2c_start(DEVICE_ADDRESS+I2C_WRITE);		   // set device address and write mode
	i2c_write(address);
	i2c_write(value);
	i2c_stop();
}

uint8_t readRegister(uint8_t reg){
	i2c_start(DEVICE_ADDRESS+I2C_WRITE);		   
	i2c_write(reg);
	i2c_start(DEVICE_ADDRESS+I2C_READ);
	uint8_t returnVal = i2c_readNak();

	return returnVal;
}

void readPitch(){
	bnogyro.Pitch =   readRegister(0x1F) << 8;
	bnogyro.Pitch |=  readRegister(0x1E);
	bnogyro.Pitch = (bnogyro.Pitch/ 16.0);
	
	char str[4];
	// TODO: change to own method, but not yet working 
	//convertIntToString(bnogyro.Pitch, 4, &str);
	sprintf(str, "%d", bnogyro.Pitch);
	serial_print_line( str);
	return bnogyro.Pitch;
}




