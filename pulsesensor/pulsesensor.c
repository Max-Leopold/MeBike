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

int valueLength = 10;
int bpmValues[10];
int average = 0;
int sum = 0, bpm = 0;
int arrayLength = 0;
int waitTime = 60;
unsigned long startMillis;
unsigned long sendStartMillis;
long lastHearbeatdetected;



void pulsesensor_init() {
    startMillis = getMillis();
	sendStartMillis = getMillis();
	//
	//ADMUX = 0;					// use #0 ADC
	//ADMUX |= (1 << REFS0);		// use AVcc as the reference
	//ADMUX &= ~(1 << ADLAR);		// clear for 10 bit resolution
//
	//ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);    // 128 prescale for 8Mhz
	//ADCSRA |= (1 << ADATE);		// Set ADC Auto Trigger Enable
	//ADCSRB = 0;					// 0 for free running mode
	//ADCSRA |= (1 << ADEN);		// Enable the ADC
	//ADCSRA |= (1 << ADIE);		// Enable Interrupts
	//ADCSRA |= (1 << ADSC);		// Start the ADC conversion
}


void pulsesensor_main() {
    char bpmValue[3];
    sprintf(bpmValue, "%d", getBpm());
	//convertIntToString(getBpm(), 3, bpmValue);

    /* if-statement in which we get the information if a heartbeat is detected or not
    ---> if yes: the value gets added to the bpmValues-array via the addValue-method
    ---> if the sensor doesn't recognize a signal within 7 seconds the output will be 0 */

    int elapsedMillis = getMillis() - startMillis;
    if (elapsedMillis >= waitTime){
        startMillis = getMillis();
        static int beatMsec = 0;

        if (heartbeatDetected(60, get_adc_current())) {

            lastHearbeatdetected = getMillis();
            bpm = 60000 / beatMsec;

            beatMsec = 0;

            addValue(bpm);
        } else {
            if (getMillis() - lastHearbeatdetected > 7000) {
	            clearBpm();
	        }
        }
        beatMsec += 60;
    }
	if(getMillis() - sendStartMillis > 1000){
		sendStartMillis = getMillis();
		bluetooth_send_pulse(bpmValue);
	}
}

//heartbeat-detection method inspired from the datasheet from joy-it
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

int getBpm() {
    return average;
}

void addValue(int bpm) {
    //for normalizing purpose
    if (bpm < 40 || bpm > 200) {
        return;
    }
    //increases the position of the values by one
    for (int i = valueLength - 1; i > 0; i--) {
        bpmValues[i] = bpmValues[i - 1];
    }
    bpmValues[0] = bpm; //put the latest value to position 0 of the array

    sum = 0;

    //calculates the average of the 10 bpm values in the array
    for (int i = 0; i < valueLength; i++) {
        sum += bpmValues[i];
    }
    if (arrayLength < valueLength) {
        arrayLength++;
    }
    average = sum / arrayLength;

}

//if no finger is on the sensor this method get called and sets all bpmValues to 0 - so if 0 is displayed no finger is on the sensor
void clearBpm() {
    for (int i = 0; i < valueLength - 1; i++) {
        bpmValues[i] = 0;
    }
    average = 0;
    arrayLength = 0;
}
