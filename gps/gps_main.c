//
// Created by Max Leopold on 07/06/2020.
//

#include "gps_main.h"
#include "../uart/softuart.h"
#include "../uart/serial.h"
#include "../bluetooth/bluetooth.h"
#include <string.h>
#include <avr/interrupt.h>

char gps_buf[100]; //Char buffer to store the GPS data String
char gps_string_received; //"boolean" char to indicate if we received new
/*
 * Delimiter to cut the GPS data String which is received via UART.
 * The GPS Sensor uses ',' to separate data within one data string.
 */
char delimiter[] = ",";

/*
 * These two variables are used to check if we received a new GPS Dataset.
 * The GPS Sensor sends several Data Strings, each starting with a unambiguous Prefix.
 * For our Use Case we are only interested in the Data String which has the Prefix $GPGGA.
 * Therefore we create a char array and a counter, which always points to the location of the
 * char we expect to receive next.
 * Example: gps_ind id = 0
 * -> We receive a '$' -> increment gps_ind to 1
 * -> We receive a 'G' -> increment gps_ind to 2
 * -> We receive a 'G' -> We expected a 'P' (Position 2 in the array), so this is not the Prefix we are
 * looking for -> Reset gps_ind to 0.
 */
char gps_ind;
char gpgga[] = {'$', 'G', 'P', 'G', 'G', 'A'};

char debugMode; //Indicate if debug logs should be printed via UART

//Global variables for GPS data
char *gmtTime;
char *latitude;
char *longitude;

void gps_init() {
    /* Initialize SoftUART interface
     * The SoftUART Interface isn't used anymore, because we switched to the dedicated
     * RX Pin of the Arduino Board. I just kept this here for reference in case we ever wanted
     * to switch back
    */
    //softuart_init();
}

//Main routine for the gps_sensor
void gps_main(char debug) {
    debugMode = debug; //Set debug mode

    if (gps_string_received) { //Check if we received new GPS Data
        gps_string_received = 0; //Set gps_string_received to 0, so we don't send the same data twice
        gps_ind = 0; //Reset the gps_ind
        create_gps_coordinates(); //Cut the GPS Data String and save assign values to the global GPS variables
        if (debugMode ==
            '1') { //If debugmode enabled, print the gps data via UART. debugmode should be disabled in production
            char str_copy[100];
            strncpy(str_copy, "Latitude: ", strlen("Latitude: "));
            strncat(str_copy, latitude, strlen(latitude));
            strncat(str_copy, ", Longitude: ", strlen(", Longitude: "));
            strncat(str_copy, longitude, strlen(longitude));
            strncat(str_copy, ", GMT Time: ", strlen(", GMT Time: "));
            strncat(str_copy, gmtTime, strlen(gmtTime));
            serial_print_line(str_copy);
        }

        bluetooth_send_gps(gmtTime, latitude, longitude); //Send the GPS Data via the Bluetooth sensor
    }
}

/*
 * This method checks if we currently are receiving the correct GPS data String.
 * As mentioned above, we are only interested in the String with the $GPGGA Prefix.
 */
void check_for_coordinates(char c) {
    gps_buf[gps_ind] = c; //Write the char to the buffer
    gps_ind++;//Increment gps_ind

    /*
     * If gps_ind is < 7, we are still checking for the Prefix. Every GPS Data String Prefix is 6 characters long.
     */
    if (gps_ind < 7) {
        /*
         * If we are still checking for a prefix and the received char is not the char we expected next in out
         * prefix, we reset the gps_ind to 0 and and start the search for the correct prefix again.
         */
        if (gps_buf[gps_ind - 1] != gpgga[gps_ind - 1]) {
            gps_ind = 0;
        }
    }

    /*
     * If gps_ind >= 50 we can be sure, that we found the correct prefix and also received enough data to cut
     * the data string into longitude, latitude and gmt time. These values + prefix will always be shorter then
     * 50 characters. Therefore we set gps_string_received to 1 after 50 values.
     */
    if (gps_ind >= 50) {
        gps_string_received = 1;
    }

}

/*
 * This method is used to "cut" the received GPS data String into latitude, longitude and gmt time and save them
 * in the global variables.
 */
void create_gps_coordinates() {
    char *ptr;
    ptr = strtok(gps_buf, delimiter); //strtok cut a string at the first occurence of a delimiter (in out case ',')

    /*
     * The data String we receive is build likt this:
     * $GPGGA,<UTC_TIME>,<LATITUDE>,N,<LONGITUDE>,W...
     * Therefore we are interested in the values between the first and second comma, second and third and
     * fourth and fifth.
     * Therefore we have to call strtok five times.
     */
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
        ptr = strtok(NULL, delimiter); //cut the String again at the next ','
    }
}

/*
 * Interrupt for the RX Pin
 */
ISR(USART_RX_vect) {
    unsigned char b;
    b = UDR0; //Read the char that was send to the RX Pin
    check_for_coordinates(b); //Check if the received char in interesting for us or not
}