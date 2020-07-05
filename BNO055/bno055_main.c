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
#include <util/delay.h>
#include <avr/io.h>
#include <math.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define DEVICE_ADDRESS (0x28 << 1)

bool debugMode;

char *temperature;
char *accelForward;
char *accelSideways;
char *pitch;

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

void bno055_main(bool debug){
	debugMode = debug;
	readPitch();
	readAcceleration();
	readTemp();
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

	int size = snprintf(NULL, 0, "%d", pitch);			// calculate needed char array length to store the value as String
	char * pitchString = malloc(size + 1);				// allocate the needed space for the string on the heap
	sprintf(pitchString, "%d", pitch);					// convert the value to a string

	pitch = &pitchString;								// save string in global variable
		
	if(debugMode){
		serial_print_line(pitchString);
	}
}

// Read all acceleration registers from the sensor. Starting from register 0x28 up to 0x2D. Values in g.
void readAcceleration(){
	i2c_start(DEVICE_ADDRESS+I2C_WRITE);
	i2c_write(0x28);
	i2c_start(DEVICE_ADDRESS+I2C_READ);
	int forw, sidew, upw;
	int forwD, sidewD, upwD;
	
	sidew =  i2c_readAck();					//reg 0x28 Linear acceleration x axis LSB
	sidew |= i2c_readAck() <<8;				//reg 0x29 Linear acceleration x axis MSB
	
	forw =  i2c_readAck();					//reg 0x2A Linear acceleration y axis LSB
	forw |= i2c_readAck() << 8;				//reg 0x2B Linear acceleration y axis MSB

	
	i2c_stop();
	
	forwD =  (int) ((double) forw) / 9.81;	//convert raw sensor data to g (9.81 m/s^2) as double value.
	sidewD = (int) ((double) sidew) /9.81;
	

	int size = snprintf(NULL, 0, "%d", forwD);	// calculate needed char array length to store the value as String
	char * forwardString = malloc(size + 1);	// allocate the needed space for the string on the heap
	sprintf(forwardString, "%d", forwD);		// convert the value to a string
	accelForward = &forwardString;				// save string in global variable
	
	size = snprintf(NULL, 0, "%d", sidewD);
	char * sideString = malloc(size + 1);
	sprintf(sideString, "%d", sidewD);
	accelSideways = &sideString;

    if(debugMode){
		char str_copy[100];
		strncpy(str_copy, "forward: ");
		strncat(str_copy, accelForward);
		strncat(str_copy, ", sideways: ");
		strncat(str_copy, accelSideways);
		serial_print_line(str_copy);
	}
}

void readTemp(){
	uint8_t temp;
	i2c_start(DEVICE_ADDRESS+I2C_WRITE);				// send start condition Sensor Address with R/W Bit (LSB, Bit 8) = 0
	i2c_write(0x34);									// write the register address. 0x34 as address of temperature Register
	i2c_start(DEVICE_ADDRESS+I2C_READ);					// send start condition Sensor Address with R/W Bit (LSB, Bit 8) = 1
	
	temp = i2c_readNak();								// read register and send NACK, so the sensor won't read the subsequent register
	i2c_stop();
	
	int size = snprintf(NULL, 0, "%d", temp);           // calculate needed char array length to store the value as String   
	char * tempString = malloc(size + 1);				// allocate the needed space for the string on the heap
	sprintf(tempString, "%d", temp);					// convert the value to a string
	temp = &tempString;									// save string in global variable
		
}