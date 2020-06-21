/*
 * Me-Bike.c
 *
 *
 * Author : Frederic Henn
 */ 
#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#include <avr/interrupt.h>
#include <stdbool.h>
#include <stdio.h>
#include <avr/io.h>
#include <stdint.h>
#include <stdlib.h>
#include <util/atomic.h>
#include "uart/serial.h"
#include "pulsesensor.h"


int adc_current_value = 0;
int adc_last_value = 0;
unsigned long millis = 0;
int bpm = 0;
unsigned long waitTime = 0;
int averageBpm = 0;
long lastHearbeatdetected = 0;


int main(void)
{
	
    sei();

	serial_init();

	ADC_init();

	while(1) {

		char bpmValue[1];
		sprintf(bpmValue, "%d", getBpm());
		
		
		struct pulse_value data;
		data.pulse = bpmValue[0];
		
		//TO-DO - send via Bluetooth
	}
}


 ISR (ADC_vect)
 {
	 adc_last_value = adc_current_value;

	 int ADCval = ADCL;
	 ADCval = (ADCH << 8) + ADCval;   

	 adc_current_value = ADCval;

	 /* if-statement in which we get the information if a heartbeat is detected or not 
	 ---> if yes: the value gets added to the bpmValues-array via the addValue-method
	 ---> if the sensor doesn't recognize a signal within 7 seconds the output will be 0 */ 
	 if(millis >= waitTime)
	 {
		 static int beatMsec = 0;
		 
		 if (heartbeatDetected(60,adc_current_value))
		 {
			
			 lastHearbeatdetected = millis;
			 bpm = 60000 / beatMsec;

			 beatMsec = 0;

			 addValue(bpm);
		 } else {

			if(millis - lastHearbeatdetected > 7000) {
				clearBpm();			
			}	
		 }
		 waitTime = millis + 60;
		 beatMsec += 60;

	 }
 }
 

 ISR (TIMER2_COMPA_vect)  // This timer counts the millis
 {
	 millis++;

 }
