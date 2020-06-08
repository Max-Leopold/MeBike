//
// Created by Max Leopold on 07/06/2020.
//

#include "gps_main.h"
#include "../uart/softuart.h"
#include "../uart/serial.h"

#include <string.h>

char gps_buf[100];
char gps_ind;
char gpgga[] = {'$', 'G', 'P', 'G', 'G', 'A'};
char gps_string_received;
char delimiter[] = ",";

void gps_init() {
    softuart_init();
}

void gps_main() {

    if (softuart_kbhit()) {
        char c = softuart_getchar();
        check_for_coordinates(c);
    }
    if (gps_string_received) {
        gps_string_received = 0;
        gps_ind = 0;
        struct gps_coordinates gpsCoordinates = create_gps_coordinates();
        char str_copy[100];
        strcpy(str_copy, "Latitude: ");
        strcat(str_copy, gpsCoordinates.latitude);
        strcat(str_copy, ", Longitude: ");
        strcat(str_copy, gpsCoordinates.longitude);
        strcat(str_copy, ", GMT Time: ");
        strcat(str_copy, gpsCoordinates.gmt_time);
        serial_print_line(str_copy);
        //TODO send data via bluetooth here
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

struct gps_coordinates create_gps_coordinates() {
    char *ptr;
    ptr = strtok(gps_buf, delimiter);
    struct gps_coordinates gpsCoordinates;

    for (int i = 0; i < 5; ++i) {
        if (i == 1) {
            gpsCoordinates.gmt_time = ptr;
        }
        if (i == 2) {
            gpsCoordinates.latitude = ptr;
        }
        if (i == 4) {
            gpsCoordinates.longitude = ptr;
        }
        ptr = strtok(NULL, delimiter);
    }

    return gpsCoordinates;
}
