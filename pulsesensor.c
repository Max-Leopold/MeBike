/*
 * pulsesensor.c
 *
 *  Author: Frederic Henn
 */ 

 #include "serial.h"
 #include <avr/io.h>
 #include <stdio.h>
 #include <util/delay.h>
 #include <stdint.h>
 #include <stdbool.h>



 int ADCsingleREAD(uint8_t adc_channel)
 {
	 int ADCval;

	 ADMUX = adc_channel;         // use #1 ADC
	 ADMUX |= (1 << REFS0);    // use AVcc as the reference
	 ADMUX &= ~(1 << ADLAR);   // clear for 10 bit resolution
	 
	 ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);    // 128 prescale for 8Mhz
	 ADCSRA |= (1 << ADEN);    // Enable the ADC

	 ADCSRA |= (1 << ADSC);    // Start the ADC conversion

	 while(ADCSRA & (1 << ADSC));      // Thanks T, this line waits for the ADC to finish


	 ADCval = ADCL;
	 ADCval = (ADCH << 8) + ADCval;    // ADCH is read so ADC can be updated again

	 return ADCval;
 }


 
 bool heartbeatDetected(int delay)
 {
	 static int maxValue = 0;
	 static bool isPeak = false;
	 
	 
	 bool result = false;
	 
	 // Hier wird der aktuelle Spannungswert am Fototransistor ausgelesen und in der rawValue-Variable zwischengespeichert
	 int ADCvalue = ADCsingleREAD(0);
	 ADCvalue *= (1000/delay);
	 
	 // Sollte der aktuelle Wert vom letzten maximalen Wert zu weit abweichen
	 // (z.B. da der Finger neu aufgesetzt oder weggenommen wurde)
	 // So wird der MaxValue resetiert, um eine neue Basis zu erhalten.
	 if (ADCvalue * 4L < maxValue) {    maxValue = ADCvalue * 0.8;  }     // Detect new peak
	 if (ADCvalue > maxValue - (1000/delay)) {
		 // Hier wird der eigentliche Peak detektiert. Sollte ein neuer RawValue groeßer sein
		 // als der letzte maximale Wert, so wird das als Spitze der aufgezeichnten Daten erkannt.
		 if (ADCvalue > maxValue) {
			 maxValue = ADCvalue;
		 }
		 // Zum erkannten Peak soll nur ein Herzschlag zugewiesen werden
		 if (isPeak == false) {
			 result = true;
		 }
		 isPeak = true;
		 } else if (ADCvalue < maxValue - (3000/delay)) {
		 isPeak = false;
		 // Hierbei wird der maximale Wert bei jeden Durchlauf
		 // etwas wieder herabgesetzt. Dies hat den Grund, dass
		 // nicht nur der Wert sonst immer stabil bei jedem Schlag
		 // gleich oder kleiner werden wuerde, sondern auch,
		 // falls der Finger sich minimal bewegen sollte und somit
		 // das Signal generell schwaecher werden wuerde.
		 maxValue-=(1000/delay);
	 }
	 return result;
 }






 






 




