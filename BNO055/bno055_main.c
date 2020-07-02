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

typedef struct bnodata BNODATA; 

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

void getBNOData(BNODATA *bnodata, bool debug){
	debugMode = debug;
	readPitch(bnodata);
	readAcceleration(bnodata);
}

//Method to read the current pitch data of the BNO055 and store it in the BNODATA struct
void readPitch(BNODATA *bnodata){
	int pitch;
	
	i2c_start(DEVICE_ADDRESS+I2C_WRITE);				// send start condition Sensor Address with R/W Bit (LSB, Bit 8) = 0
	i2c_write(0x1E);									// write the register address. 0x1E as LSB address of Pitch data Register
	i2c_start(DEVICE_ADDRESS+I2C_READ);					// send start condition Sensor Address with R/W Bit (LSB, Bit 8) = 1
														
	pitch =  i2c_readAck();								// read register and send ACK. Sensor will automatically send subsequent register 0x1F. Return value is LSB of pitch data and will be saved at bits 0 to 7
	pitch |= i2c_readNak()<<8;							// read register and send NACK, so the sensor won't read the subsequent register. Shift result by 8 bits, so the MSB part is at bits 8 to 15
	i2c_stop();											// stop the i2c communication

	pitch = (pitch/ 16.0);

	int size = snprintf(NULL, 0, "%d", pitch);
	char * pitchString = malloc(size + 1);
	sprintf(pitchString, "%d", pitch);

	bnodata->Pitch = pitchString;
	
	if(debugMode){
		serial_print_line(pitchString);
	}
}

// Read all acceleration registers from the sensor. Starting from register 0x28 up to 0x2D. Values in g.
void readAcceleration(BNODATA *bnodata){
	i2c_start(DEVICE_ADDRESS+I2C_WRITE);
	i2c_write(0x28);
	i2c_start(DEVICE_ADDRESS+I2C_READ);
	int forw, sidew, upw;
	int forwD, sidewD, upwD;
	
	sidew =  i2c_readAck();					//reg 0x28 Linear acceleration x axis LSB
	sidew |= i2c_readAck() <<8;				//reg 0x29 Linear acceleration x axis MSB
	
	forw =  i2c_readAck();					//reg 0x2A Linear acceleration y axis LSB
	forw |= i2c_readNak() << 8;				//reg 0x2B Linear acceleration y axis MSB
	
	i2c_stop();
	
	forwD =  (int) ((double) forw) / 9.81;	//convert raw sensor data to g (9.81 m/s^2) as double value.
	sidewD = (int) ((double) sidew) /9.81;

	int size = snprintf(NULL, 0, "%d", forwD);
	char * forwardString = malloc(size + 1);
	sprintf(forwardString, "%d", forwD);
	bnodata->accelForwards = forwardString;
	
	size = snprintf(NULL, 0, "%d", sidewD);
	char * sideString = malloc(size + 1);
	sprintf(sideString, "%d", sidewD);
	bnodata->accelSideways = sideString;

    if(debugMode){
		char str_copy[100];
		strcpy(str_copy, "forward: ");
		strcat(str_copy, bnodata->accelForwards);
		strcat(str_copy, ", sideways: ");
		strcat(str_copy, bnodata->accelSideways);
		serial_print_line(str_copy);
	}
}