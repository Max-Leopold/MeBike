/*
 * timer.c
 *
 * Created: 28.06.2020 16:50:23
 *  Author: Wanja
 */ 

#include "timer.h"
#include <avr/interrupt.h>

static unsigned long millis = 0;

void initMillis(){
	// Enable timer 2 to count the millis
	// Set the Timer Mode to CTC
	TCCR2A |= (1 << WGM21);
	// Count every milli second ((16*10^6 / 256) * 0,001) -1
	OCR2A = 61;
	TIMSK2 |= (1 << OCIE2A);    //Set the ISR COMPA vect
	TCCR2B |= (1 << CS22) | (1 << CS21); // set prescaler to 256 and start the timer
}

unsigned long getMillis(){
	return millis;
}

ISR (TIMER2_COMPA_vect) {  // This timer counts the millis
	millis++;
}

