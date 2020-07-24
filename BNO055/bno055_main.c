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
#include "../bluetooth/bluetooth.h"
#include "../uart/serial.h"
#include "../i2cmaster/i2cmaster.h"
#include "../util/Interrupt/timer.h"
#include <util/delay.h>
#include <avr/io.h>
#include <math.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define DEVICE_ADDRESS (0x28 << 1)

char debugMode;
long bnoStartMillis;

char temperature[4];								// String to store the read temperature data
char accelForward[4];								// String to store the read acceleration data in the driving direction
char accelSideways[4];								// String to store the read acceleration data to the sides of the driving direction
char pitch[4];										// String to store the read pitch data of the sensor

// init method, only called once on boot of system. Initializes the i2c communication and sets the operation mode NDOF for BNO055.
void bno_init(){

    i2c_init();									    // init I2C interface
	_delay_ms(1000);							    // important, so the sensor has enough time to boot properly, before setting the operation mode 
	
    /* write 0x50 As device address and add as 8th bit a 0 to write or 1 to read */
	//Set NDOF mode
    i2c_start_wait(DEVICE_ADDRESS+I2C_WRITE);       // set device address and write mode
	i2c_write(0x3D);								// Mode Selection Register
	i2c_write(0x0C);								// set NDOF mode to use accelerometer and gyroscope
	i2c_stop();										// stop i2c communication
	bnoStartMillis = getMillis();					// set the initial time 
}

// main method, called on every iteration through main loop. Calls all methods which will retrieve the current data from BNO055 and transmition via bluetooth.
void bno055_main(char debug) {
    debugMode = debug;								// defines if debug mode is active
    readPitch();
    readAcceleration();
	readTemp();
	sendBluetooth();
}


// method to read the current pitch data from BNO055
void readPitch(){
	int _pitch;
		
	i2c_start(DEVICE_ADDRESS+I2C_WRITE);				// send start condition Sensor Address with R/W Bit (LSB, Bit 8) = 0
	i2c_write(0x1E);									// write the register address. 0x1E as LSB address of Pitch data Register
	i2c_start(DEVICE_ADDRESS+I2C_READ);					// send start condition Sensor Address with R/W Bit (LSB, Bit 8) = 1													
	_pitch =  i2c_readAck();							// read register and send ACK. Sensor will automatically send subsequent register 0x1F. Return value is LSB of pitch data and will be saved at bits 0 to 7
	_pitch |= i2c_readNak()<<8;							// read register and send NACK, so the sensor won't read the subsequent register. Shift result by 8 bits, so the MSB part is at bits 8 to 15
	i2c_stop();											// stop i2c communication

	_pitch = (_pitch/ 16.0);							// pitch value divided by 16 to get correct value in degrees

	sprintf(pitch, "%d", _pitch);						// convert the value to a string
		
	if(debugMode == '1'){
		serial_print_line(pitch);						// if debug mode is activated, print out the pitch of the sensor
	}
}

// method to read all acceleration registers from BNO055. Starting from register 0x28 up to 0x2D. Values in g.
void readAcceleration(){
	i2c_start(DEVICE_ADDRESS+I2C_WRITE);	// send start condition Sensor Address with R/W Bit (LSB, Bit 8) = 0
	i2c_write(0x28);						// write the register address. 0x28 as address of linear acceleration of the X-Axis (LSB)
	i2c_start(DEVICE_ADDRESS+I2C_READ);		// send start condition Sensor Address with R/W Bit (LSB, Bit 8) = 1
	int forw, sidew, upw;					// local variables to temporarily store the read data
	int forwD, sidewD, upwD;
	sidew =  i2c_readAck();					// reg 0x28 Linear acceleration x axis LSB
	sidew |= i2c_readAck() <<8;				// reg 0x29 Linear acceleration x axis MSB
	
	forw =  i2c_readAck();					// reg 0x2A Linear acceleration y axis LSB
	forw |= i2c_readNak() << 8;				// reg 0x2B Linear acceleration y axis MSB
	i2c_stop();								// stop i2c communication
	forwD =  (int) ((double) forw) / 9.81;	// convert raw sensor data to g (9.81 m/s^2) as double value.
	sidewD = (int) ((double) sidew) /9.81;
	
	
	sprintf(accelForward, "%d", forwD);		// convert the value to a string	
    sprintf(accelSideways, "%d", sidewD);
	
    if(debugMode == '1'){					// if debug mode is activated, print out the acceleration values of the sensor
		char str_copy[100];
		strncpy(str_copy, "forward: ", strlen("forward: "));
		strncat(str_copy, accelForward, strlen(accelForward));
		strncat(str_copy, ", sideways: ", strlen(", sideways: "));
		strncat(str_copy, accelSideways, strlen(accelSideways));
		serial_print_line(str_copy);
	}
}

// method to read temperature value from BNO055
void readTemp(){
	int temp;
	i2c_start(DEVICE_ADDRESS+I2C_WRITE);				// send start condition Sensor Address with R/W Bit (LSB, Bit 8) = 0
	i2c_write(0x34);									// write the register address. 0x34 as address of temperature Register
	i2c_start(DEVICE_ADDRESS+I2C_READ);					// send start condition Sensor Address with R/W Bit (LSB, Bit 8) = 1
	
	temp = i2c_readNak();								// read register and send NACK, so the sensor won't read the subsequent register
	i2c_stop();											// stop i2c communication
	
	sprintf(temperature, "%d", temp);					// convert the value to a string
}

// method to send all read values from BNO055 via bluetooth
void sendBluetooth(){
	if(getMillis() - bnoStartMillis > 1000){						  // only send if one second has passed
		bnoStartMillis = getMillis();								  // reset initial time 
		bluetooth_send_gyro(accelForward, accelSideways, pitch);	  // send gyro values
		bluetooth_send_temperature(temperature);					  // send temperature value
	}
}