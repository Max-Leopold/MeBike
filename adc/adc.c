//
// Created by hennf on 22.06.2020.
//

#include "adc.h"
#include <avr/io.h>

void ADC_init() {
    ADMUX = 0;         // use #0 ADC
    ADMUX |= (1 << REFS0);    // use AVcc as the reference
    ADMUX &= ~(1 << ADLAR);   // clear for 10 bit resolution

    ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);    // 128 prescale for 8Mhz
    ADCSRA |= (1 << ADATE);   // Set ADC Auto Trigger Enable
    ADCSRB = 0;               // 0 for free running mode
    ADCSRA |= (1 << ADEN);    // Enable the ADC
    ADCSRA |= (1 << ADIE);    // Enable Interrupts
    ADCSRA |= (1 << ADSC);    // Start the ADC conversion


    // Enable timer 2 to count the millis
    // Set the Timer Mode to CTC
    TCCR2A |= (1 << WGM21);
    // Count every milli second ((16*10^6 / 256) * 0,001) -1
    OCR2A = 61;
    TIMSK2 |= (1 << OCIE2A);    //Set the ISR COMPA vect
    TCCR2B |= (1 << CS22) | (1 << CS21); // set prescaler to 256 and start the timer

}