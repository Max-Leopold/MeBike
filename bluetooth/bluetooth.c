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

size_t BOM_SIZE;
size_t EOM_SIZE;
size_t DELIMITER_SIZE;


void bluetooth_init(){
	BOM_SIZE = sizeof(MESSAGE_BEGIN);
	EOM_SIZE = sizeof(MESSAGE_END);
	DELIMITER_SIZE = sizeof(DATA_DELIMITER);
}

/************************************************************************/
/* Sends the gps struct data                                            */
/************************************************************************/
void bluetooth_send_gps(struct gps_coordinates coords){
	char data[100];
	strncpy(data, "<BOM>", BOM_SIZE);
	strncat(data, "gps", strlen("gps"));
	strncat(data, DATA_DELIMITER, DELIMITER_SIZE);
	strncat(data, coords.gmt_time, strlen(coords.gmt_time));
	strncat(data, DATA_DELIMITER, DELIMITER_SIZE);
	strncat(data, coords.latitude, strlen(coords.latitude));
	strncat(data, DATA_DELIMITER, DELIMITER_SIZE);
	strncat(data, coords.longitude, strlen(coords.longitude));
	strncat(data, "<EOM>", EOM_SIZE);
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