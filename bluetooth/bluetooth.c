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
void bluetooth_send_gps(char *gmt_time, char *latitude, char *longitude){
	char data[100];
	strncpy(data, "<BOM>", BOM_SIZE);
	strncat(data, "gps", strlen("gps"));
	strncat(data, DATA_DELIMITER, DELIMITER_SIZE);
	strncat(data, gmt_time, strlen(gmt_time));
	strncat(data, DATA_DELIMITER, DELIMITER_SIZE);
	strncat(data, latitude, strlen(latitude));
	strncat(data, DATA_DELIMITER, DELIMITER_SIZE);
	strncat(data, longitude, strlen(longitude));
	strncat(data, "<EOM>", EOM_SIZE);
	serial_print_line(data);
}


void bluetooth_send_temperature(char *temperature){
	char data[100];
	strncpy(data, "<BOM>", BOM_SIZE);
	strncat(data, "temp", strlen("temp"));
	strncat(data, DATA_DELIMITER, DELIMITER_SIZE);
	strncat(data, temperature, strlen(temperature));
	strncat(data, "<EOM>", EOM_SIZE);
	serial_print_line(data);
}

void bluetooth_send_pulse(char *pulse){
	// TODO
	char data[100];
	strncpy(data, "<BOM>", BOM_SIZE);
	strncat(data, "pulse", strlen("pulse"));
	strncat(data, DATA_DELIMITER, DELIMITER_SIZE);
	strncat(data, pulse, strlen(pulse));
	strncat(data, "<EOM>", EOM_SIZE);
	serial_print_line(data);
}

void bluetooth_send_gyro(char *accFwd, char *accSd, char *pitch){
	char data[100];
	strncpy(data, "<BOM>", BOM_SIZE);
	strncat(data, "gyro", strlen("gyro"));
	strncat(data, DATA_DELIMITER, DELIMITER_SIZE);
	strncat(data, pitch, strlen(pitch));
	strncat(data, DATA_DELIMITER, DELIMITER_SIZE);
	strncat(data, accFwd, strlen(accFwd));
	strncat(data, DATA_DELIMITER, DELIMITER_SIZE);
	strncat(data, accSd, strlen(accSd));
	strncat(data, "<EOM>", EOM_SIZE);
	serial_print_line(data);
}

void bluetooth_send_rpm(char *rpm){
	char data[50];
	strncpy(data, "<BOM>", BOM_SIZE);
	strncat(data, "rpm", strlen("rpm"));
	strncat(data, DATA_DELIMITER, DELIMITER_SIZE);
	strncat(data, rpm, strlen(rpm));
	strncat(data, "<EOM>", EOM_SIZE);
	serial_print_line(data);
}