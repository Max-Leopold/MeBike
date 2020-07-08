//
// Created by Max Leopold on 07/06/2020.
//

#include "gps_main.h"
#include "../uart/softuart.h"
#include "../uart/serial.h"
#include "../bluetooth/bluetooth.h"
#include <string.h>
#include <avr/interrupt.h>

char gps_buf[100];
char gps_ind;
char gpgga[] = {'$', 'G', 'P', 'G', 'G', 'A'};
char gps_string_received;
char delimiter[] = ",";

char debugMode;

char *gmtTime;
char *latitude;
char *longitude;

void gps_init() {
    softuart_init();
}

void gps_main(char debug) {
    debugMode = debug;

    if (gps_string_received) {
        gps_string_received = 0;
        gps_ind = 0;
        create_gps_coordinates();
        if (debugMode == '1') {
            char str_copy[100];
            strncpy(str_copy, "Latitude: ", strlen("Latitude: "));
            strncat(str_copy, latitude, strlen(latitude));
            strncat(str_copy, ", Longitude: ", strlen(", Longitude: "));
            strncat(str_copy, longitude, strlen(longitude));
            strncat(str_copy, ", GMT Time: ", strlen(", GMT Time: "));
            strncat(str_copy, gmtTime, strlen(gmtTime));
            serial_print_line(str_copy);
        }

		bluetooth_send_gps(gmtTime, latitude, longitude);
    }
}

void check_for_coordinates(char c) {
    gps_buf[gps_ind] = c;
    gps_ind++;

    if (gps_ind < 7) { // Because of format $GPGGA
        if (gps_buf[gps_ind - 1] != gpgga[gps_ind - 1]) {
            gps_ind = 0;
        }
    }
    if (gps_ind >= 50) {
        gps_string_received = 1;
    }

}

void create_gps_coordinates() {
    char *ptr;
    ptr = strtok(gps_buf, delimiter);

    for (int i = 0; i < 5; ++i) {
        if (i == 1) {
            gmtTime = ptr;
        }
        if (i == 2) {
            latitude = ptr;
        }
        if (i == 4) {
            longitude = ptr;
        }
        ptr = strtok(NULL, delimiter);
    }
}

ISR(USART_RX_vect)
{
    unsigned char b;
    b=UDR0;
    check_for_coordinates(b);
}