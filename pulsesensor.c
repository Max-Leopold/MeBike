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

	 ADMUX = adc_channel;      // verwende den gegebenen ADC_Channel z.B. 0 = ADC0 also PIN A0
	
	 ADMUX |= (1 << REFS0);    // verwende AVcc als Referenzspannung
	 
	 ADMUX &= ~(1 << ADLAR);   // clear for 10 bit resolution
	 
	 ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);    // 128 als Prescaler(Frequenzvorteiler) verwenden, da 16 MHz / 200kHz = 80 - nächst höherer Wert ist 128 (siehe Tabelle) 
	
	 ADCSRA |= (1 << ADEN);    // ADC aktivieren

	 ADCSRA |= (1 << ADSC);    // Eine ADC-Konvertierung

	 while(ADCSRA & (1 << ADSC));      // auf Abschluss der Konvertierung warten

	 ADCval = ADCL;
	 ADCval = (ADCH << 8) + ADCval;    // ADCH is read so ADC can be updated again

	 return ADCval;				
 }

 
 bool heartbeatDetected(int delay)
 {
	 static int maxValue = 0;
	 static bool isPeak = false;
	 
	 
	 bool result = false;
	 
	 // Hier wird der aktuelle Spannungswert am Fototransistor ausgelesen und in der ADCvalue-Variable zwischengespeichert
	 int ADCvalue = ADCsingleREAD(0);
	 ADCvalue *= (1000/delay);
	 
	 /* Sollte der aktuelle ADC-Wert vom letzten maximalen Wert zu weit abweichen
	  (z.B. da der Finger neu aufgesetzt oder weggenommen wurde) */

	 if (ADCvalue * 4L < maxValue) {    maxValue = ADCvalue * 0.8;  }     // Detect new peak
	 if (ADCvalue > maxValue - (1000/delay)) {
		 // Hier wird der eigentliche Peak detektiert. Sollte ein neuer ADCValue groeßer sein
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






 






 




