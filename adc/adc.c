//
// Created by hennf on 22.06.2020.
//

#include "adc.h"
#include <avr/io.h>
#include <avr/interrupt.h>

int adc_current_value = 0; //Integer to store the currently read adc value

//init method for the analog-digital converter
void ADC_init()
{
    ADMUX = 0;         // use #0 ADC
    ADMUX |= (1 << REFS0);    // use AVcc as the reference
    ADMUX &= ~(1 << ADLAR);   // clear for 10 bit resolution

    ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);    // Use 128 prescaler for 16Mhz
    ADCSRA |= (1 << ADATE);   // Set ADC Auto Trigger Enable
    ADCSRB = 0;               // 0 for free running mode - free running means: The ADC starts automatically a new measurement after the last one
    ADCSRA |= (1 << ADEN);    // Must be set to enable the ADC
    ADCSRA |= (1 << ADIE);    // Enable Interrupts
    ADCSRA |= (1 << ADSC);    // Start the ADC conversion
}

//get method to return the current adc value
int get_adc_current(){
	return adc_current_value;
}

//Interrupt for the ADC
ISR (ADC_vect)
{
    //When a conversion is complete, the measured value is in these two registers (ADCL and ADCH)
	int ADCval = ADCL; //Integer to store the value from register ADCL
	ADCval = (ADCH << 8) + ADCval; //To read the value of the ADCH register plus the value of ADCL

	adc_current_value = ADCval; //set adc_current_value to the read adc value
}