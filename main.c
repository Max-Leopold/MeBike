/*
 * Me-Bike.c
 *
 *
 * Author : Frederic Henn
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdbool.h>
#include <stdio.h>
#include <avr/io.h>
#include <stdint.h>
#include "uart/serial.h"
#include "pulsesensor.h"
#include <stdlib.h>
#include <util/atomic.h>
#include "dht11.h" //sp?ter in DHT11-Branch packen

#include "time.h"
#define samp_siz 20

volatile unsigned long timer1_millis;

 int UpperThreshold = 518;
 int LowerThreshold = 490;
 bool IgnoreReading = false;
 bool FirstPulseDetected = false;
 //unsigned long FirstPulseTime = 0;
 //unsigned long SecondPulseTime = 0;

 unsigned long PulseInterval = 0;


uint8_t I_RH,D_RH,I_Temp,D_Temp,CheckSum; //sp?ter in DHT11-Branch packen

const int delayMsec = 60;

clock_t FirstPulseTime, SecondPulseTime;


 // Relevant
 int adc_current_value = 0;
 int adc_last_value = 0;
 bool rising = false;
 bool firstPeakDetected = false;
 unsigned long millis = 0;
 int bpm = 0;
 unsigned long last_peak_time;

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

		/*
		For testing stuff
		//serial_print("BPM: ");
		serial_print_line(bpmValue);
		_delay_ms(1000);
		*/

	}
}


 ISR (ADC_vect)
 {
	 adc_last_value = adc_current_value;

	 int ADCval = ADCL;
	 ADCval = (ADCH << 8) + ADCval;    // ADCH is read so ADC can be updated again

	 adc_current_value = ADCval;

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


 ISR (TIMER2_COMPA_vect)  // timer 2 counts the millis
 {
	 millis++;

 }
