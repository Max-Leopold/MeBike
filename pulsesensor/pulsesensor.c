/*
 * pulsesensor.c
 *
 *  Author: Frederic Henn
 */

#include "../uart/serial.h"
#include "../util/util.h"
#include "pulsesensor.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "../adc/adc.h"
#include "../util/Interrupt/timer.h"

int bpmValues[3];
int average = 0;
int sum = 0;
int arrayLength = 0;
struct pulse_value pulse;
unsigned long startMillis;
int waitTime = 60;


void pulsesensor_init() {
    startMillis = getMillis();
}


void pulsesensor_main() {
    char bpmValue[3] = {0};
    convertIntToString(getBpm(), 3, bpmValue);
    pulse = bpmValue;

    /* if-statement in which we get the information if a heartbeat is detected or not
    ---> if yes: the value gets added to the bpmValues-array via the addValue-method
    ---> if the sensor doesn't recognize a signal within 7 seconds the output will be 0 */

    int elapsedMillis = getMillis() - startMillis;
    if (elapsedMillis >= waitTime){
        startMillis = getMillis();
        static int beatMsec = 0;

        if (heartbeatDetected(60, adc_current_value)) {

            lastHearbeatdetected = millis;
            bpm = 60000 / beatMsec;

            beatMsec = 0;

            addValue(bpm);
        } else {

            if (millis - lastHearbeatdetected > 7000) {
                clearBpm();
            }
        }
        beatMsec += 60;

    }



}

//heartbeat-detection method inspired from the datasheet from joy-it
char heartbeatDetected(int delay, int ADCvalue) {
    static int maxValue = 0;
    static char isPeak = 0;

    char result = 0;

    // Hier wird der aktuelle Spannungswert am Fototransistor ausgelesen und in der ADCvalue-Variable zwischengespeichert
    ADCvalue *= (1000 / delay);

    /* Sollte der aktuelle ADC-Wert vom letzten maximalen Wert zu weit abweichen
     (z.B. da der Finger neu aufgesetzt oder weggenommen wurde) */

    if (ADCvalue * 4L < maxValue) { maxValue = ADCvalue * 0.8; }     // Detect new peak
    if (ADCvalue > maxValue - (1000 / delay)) {
        // Hier wird der eigentliche Peak detektiert. Sollte ein neuer ADCValue groe�er sein
        // als der letzte maximale Wert, so wird das als Spitze der aufgezeichnten Daten erkannt.
        if (ADCvalue > maxValue) {
            maxValue = ADCvalue;
        }
        // Zum erkannten Peak soll nur ein Herzschlag zugewiesen werden
        if (isPeak == 0) {
            result = 1;
        }
        isPeak = 1;
    } else if (ADCvalue < maxValue - (3000 / delay)) {
        isPeak = 0;
        // Hierbei wird der maximale Wert bei jeden Durchlauf
        // etwas wieder herabgesetzt. Dies hat den Grund, dass
        // nicht nur der Wert sonst immer stabil bei jedem Schlag
        // gleich oder kleiner werden wuerde, sondern auch,
        // falls der Finger sich minimal bewegen sollte und somit
        // das Signal generell schwaecher werden wuerde.
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
    //increases the position of the values ??by one
    for (int i = 2; i > 0; i--) {
        bpmValues[i] = bpmValues[i - 1];
    }
    bpmValues[0] = bpm; //put the latest value to position 0 of the array

    sum = 0;

    //calculates the average of the 3 bpm values in the array
    for (int i = 0; i < 3; i++) {
        sum += bpmValues[i];
    }
    if (arrayLength < 3) {
        arrayLength++;
    }
    average = sum / arrayLength;

}

//if no finger is on the sensor this method get called and sets all bpmValues to 0 - so if 0 is displayed no finger is on the sensor
void clearBpm() {
    for (int i = 0; i < 10; i++) {
        bpmValues[i] = 0;
    }
    average = 0;
    arrayLength = 0;
}

void interrupt_init()
{
    // Enable timer 2 to count the millis
    // Set the Timer Mode to CTC
    TCCR2A |= (1 << WGM21);
    // Count every milli second ((16*10^6 / 256) * 0,001) -1
    OCR2A = 61;
    TIMSK2 |= (1 << OCIE2A);    //Set the ISR COMPA vect
    TCCR2B |= (1 << CS22) | (1 << CS21); // set prescaler to 256 and start the timer
}

