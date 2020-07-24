/*
 * pulsesensor.c
 *
 *  Author: Frederic Henn
 */

#include "../uart/serial.h"
#include "../util/util.h"
#include "../adc/adc.h"
#include "../util/Interrupt/timer.h"
#include "pulsesensor.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>


int valueLength = 8;        /* Defines the length of the bpmValues-Array instead of using "array.length"-method.
                             Is used in the loops, so it´s not hardcoded and can be changed easily. */
int bpmValues[8];           //Array with a length of 8 to store the bpm values in it
int average = 0;            /* Integer to store the average of the read bpm values
                             * Is used to display the average of the last 8 read bpm values */
int sum = 0;                /* Integer to store the sum of the bpm´s to calculate the average
                             * The last 8 bpm values ​​are added up here                                             */
int bpm = 0;                //Integer to store value of the beats per minute.
int arrayLength = 0;        //Integer the stores the amount of the bpm values in the array to calculate the average
int waitTime = 60;          //Integer to store the wait time to realize delay functionality
unsigned long startMillis;  //Timestamps variable to realize delay functionality. For example is used to calculate the elapsed time
unsigned long sendStartMillis; //Timestamps variable for delay purposes
long lastHearbeatdetected;  //To store the time the last heartbeat was detected


//Init method to initialize
void pulsesensor_init() {
    startMillis = getMillis();      //Set the initial time for the pulse sensor
	sendStartMillis = getMillis();  //Set the inital time for the bluetooth sensor
}


void pulsesensor_main() {
    char bpmValue[4];
    sprintf(bpmValue, "%d", getBpm());
	//convertIntToString(getBpm(), 3, bpmValue);

    /* if-statement in which we get the information if a heartbeat is detected or not
    ---> if yes: the value gets added to the bpmValues-Array via the addValue-method
    ---> if the sensor doesn't recognize a signal within 7 seconds the output will be 0 */

<<<<<<< HEAD
    int elapsedMillis = getMillis() - startMillis;      /*To calculate the elapsed time between the current time
                                                         * (getMillis) and the the measured time (startMillis) */
    if (elapsedMillis >= waitTime){                     /* Query if the elapsed time is greater than or equal to the
                                                         * waiting time to get delay functionality */
        startMillis = getMillis();
        static int beatMsec = 0;

        if (heartbeatDetected(waitTime, get_adc_current())) {

            lastHearbeatdetected = getMillis(); //Set the last time a heartbeat was detected
            bpm = 60000 / beatMsec;

            beatMsec = 0;

            addValue(bpm);                      //Add the bpm value to the bpmValues-Array
        } else {
            if (getMillis() - lastHearbeatdetected > 7000) {
	            clearBpm();
	        }
        }
        beatMsec += waitTime;
    }
	if(getMillis() - sendStartMillis > 1000){ //To send the bpmValue every second
		sendStartMillis = getMillis();
		bluetooth_send_pulse(bpmValue);
	}
}

//Heartbeat-detection method inspired from the datasheet from joy-it
char heartbeatDetected(int delay, int ADCvalue) {
    static int maxValue = 0;
    static char isPeak = 0;

    char result = 0;

    // The current voltage value is read from the phototransistor and temporarily stored in the ADCvalue variable
    ADCvalue *= (1000 / delay);

    /* If the current ADC value differs too much from the last maximum value
     (e.g. because the finger was put on again or removed) */

    if (ADCvalue * 4L < maxValue) { maxValue = ADCvalue * 0.8; }     // Detect new peak
    if (ADCvalue > maxValue - (1000 / delay)) {
        /* Here the actual peak gets detected.
         * If a new ADC-Value is larger as the last maximum value, this value gets recognized as the peak of the recorded data */
        if (ADCvalue > maxValue) {
            maxValue = ADCvalue;
        }
        // Only one heartbeat should be hit for the detected peak
        if (isPeak == 0) {
            result = 1;
        }
        isPeak = 1;
    } else if (ADCvalue < maxValue - (3000 / delay)) {
        isPeak = 0;
        /* Here the maximum value gets slightly reduced again with each pass. */
        maxValue -= (1000 / delay);
    }
    return result;
}
//Returns the average of the bpm values
int getBpm() {
    return average;
}
//Adds bpm value to the bpmValues-Array
void addValue(int bpm) {
    //Normalize the values - values under 40 and over 200 will not be saved in the bpmValues-Array
    if (bpm < 40 || bpm > 200) {
        return;
    }
    //Increases the position of the values by one
    for (int i = valueLength - 1; i > 0; i--) {
        bpmValues[i] = bpmValues[i - 1];
    }
    bpmValues[0] = bpm; //Put the latest value to position 0 of the array

    sum = 0;

    //Calculates the average of the 10 bpm values in the array
    for (int i = 0; i < valueLength; i++) {
        sum += bpmValues[i]; //Increases the sum with the values of the array
    }
    if (arrayLength < valueLength) { //Increases the arrayLength if the arrayLength is lower than the valueLength
        arrayLength++;
    }
    average = sum / arrayLength; //To calculate the average

}

/* If no finger is on the sensor this method get called and sets all bpmValues to 0 - so if 0 is displayed no finger is on the sensor
   Sensor is very sensitive and sometimes reads values ​​even though there is no finger on it */
void clearBpm() {
    for (int i = 0; i < valueLength - 1; i++) {
        bpmValues[i] = 0;
    }
    average = 0; //Set average to 0
    arrayLength = 0; //Set arrayLength to 0
}