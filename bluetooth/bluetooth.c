/*
 * bluetooth.c
 *
 * Created: 10.06.2020 10:58:27
 *  Author: Flo
 */ 


#include "bluetooth.h"
#include "../uart/serial.h"
#include "../gps/gps_main.h"
#include <string.h>


/************************************************************************/
/* Sends the gps struct data                                            */
/************************************************************************/
void bluetooth_send_gps(struct gps_coordinates coords){
	char data[100];
	strncpy(data, "<BOM>", 5);
	strncat(data, "gps", 3);
	strncat(data, DATA_DELIMITER, 1);
	strncat(data, coords.gmt_time, strlen(coords.gmt_time));
	strncat(data, DATA_DELIMITER, 1);
	strncat(data, coords.latitude, strlen(coords.gmt_time));
	strncat(data, DATA_DELIMITER, 1);
	strncat(data, coords.longitude, strlen(coords.gmt_time));
	strncat(data, "<EOM>", 5);
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