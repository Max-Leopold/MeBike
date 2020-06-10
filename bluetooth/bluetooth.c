/*
 * bluetooth.c
 *
 * Created: 10.06.2020 10:58:27
 *  Author: Flo
 */ 

#define DATA_DELIMITER "|"

#include "bluetooth.h"
#include "../uart/serial.h"
#include "../gps/gps_main.h"
#include <string.h>


/************************************************************************/
/* Bluetooth uses the serial interface                                  */
/************************************************************************/
void bluetooth_init(){
	serial_init();
}

/************************************************************************/
/* Sends the gps struct data                                            */
/************************************************************************/
void bluetooth_send_gps(struct gps_coordinates coords){
	char data[100];
	strcpy(data, "<BOM>");
	strcat(data, "gps");
	strcat(data, DATA_DELIMITER);
	strcat(data, coords.gmt_time);
	strcat(data, DATA_DELIMITER);
	strcat(data, coords.latitude);
	strcat(data, DATA_DELIMITER);
	strcat(data, coords.longitude);
	strcat(data, "<EOM>");
	serial_print(data);
}


void bluetooth_send_temperature(){
	// TODO	
}

void bluetooth_send_pulse(){
	// TODO
}

void bluetooth_send_gyro(){
	// TODO
}