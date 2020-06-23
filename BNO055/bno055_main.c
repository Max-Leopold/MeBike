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
#include "bno055.h"
#include "../i2cmaster/i2cmaster.h"
#include <util/delay.h>
#include <avr/io.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
//#include <stdlib.h>

#define DEVICE_ADDRESS 0x50
struct BNOGYRO bnogyro;

char *convertIntToString(int number, int maxIntLength, char *buffer) {

	buffer[maxIntLength] = '\0';
	for (int i = maxIntLength - 1; i >= 0; --i) {
		buffer[i] = (number % 10) + '0';
		number = number / 10;
	}

	return buffer;
}

bno_init(){
	
	unsigned char ret;
    
    DDRB  = 0xff;                              // use all pins on port B for output 
    PORTB = 0xff;                              // (active low LED's )

    i2c_init();                                // init I2C interface
	_delay_ms(700);							   // important, so the sensor has enough time to boot properly, before setting the operation mode
//working
    /* write 0x50 As device address and add as 8th bit a 0 to write or 1 to read */
	//Set ACCGYRO mode
    i2c_start_wait(DEVICE_ADDRESS+I2C_WRITE);       // Set device address and write mode
	i2c_write(0x3D);								// Mode Selection Register
	i2c_write(0x05);								// Set ACCGYRO mode to use accelerometer and gyroscope
	i2c_stop();
	//_delay_ms(1000);
	
	
	printRegister(0x3D);
	printRegister(0);
	printRegister(1);
	printRegister(2);
	printRegister(3);
	
	//set Units for measurements (Register 0x3B)
	
	
}

void bno055_main(){
printRegister(0x34);						       // set device address and write mode
updateEuler();
}

void printRegister(uint8_t address){
	i2c_start_wait(DEVICE_ADDRESS+I2C_WRITE);		   // set device address and write mode
	i2c_write(address);
	i2c_start(DEVICE_ADDRESS+I2C_READ);
	uint8_t rtn = i2c_readNak();

	char str[100];
	sprintf(str, "%d", rtn);
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
	
	//if(print){
		//char str[100];
		//sprintf(str, "%d", returnVal);
		//serial_print_line( str);
	//}
	return returnVal;
}

void updateEuler(){
	//raw_x = ((uint8_t)i2c_read_ack())<<8;
	//raw_x |= i2c_read_ack();
	//for(uint8_t i = 0; i <= 255; i++){
		//char str[10000];
		//sprintf(str, "%d", readRegister(0x8));
		//strcpy(str, "register: ");
		//strcat(str, i);
		//strcat(str, readRegister(i));
		//serial_print_line( str);
	//}
	
	bnogyro.eulerPitch =   readRegister(0x15) << 8;
	bnogyro.eulerPitch |=  readRegister(0x1F);
	
	char str[10];
	sprintf(str, "%d", readRegister(0x8));
	serial_print_line( str);
	
}




