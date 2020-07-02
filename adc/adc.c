//
// Created by hennf on 22.06.2020.
//

#include "adc.h"
#include <avr/io.h>
#include <avr/interrupt.h>

int adc_current_value = 0;
int adc_last_value = 0;

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
}

//Interrupts

int getAdcValue() {
    return adc_current_value;
}

ISR (ADC_vect) {

        adc_last_value = adc_current_value;

        int ADCval = ADCL;
        ADCval = (ADCH << 8) + ADCval;

        adc_current_value = ADCval;
}
