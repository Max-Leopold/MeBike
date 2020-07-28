//
// Created by hennf on 22.06.2020.
//

#include "adc.h"
#include <avr/io.h>
#include <avr/interrupt.h>


int adc_current_value = 0; //The last adc value used to find a peak to recognize a heartbeat

//The ADC converts the analog input from the sensor to a digital value which we store in the variable adc_current_value
//init method for the analog-digital converter
void ADC_init()
{
    ADMUX = 0;         // use #0 ADC
    ADMUX |= (1 << REFS0);    // use AVcc as the reference
    ADMUX &= ~(1 << ADLAR);   // clear for 10 bit resolution

    ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);    // Use 128 prescaler for 16Mhz
    ADCSRA |= (1 << ADATE);   // Set ADC Auto Trigger Enable
    ADCSRB = 0;               /* to set the free running mode
                              * free running means: The ADC starts automatically a new measurement after the last one
                              * For example an analog value is read in. In this mode, the measurements do not have to be triggered individually like in the single conversion mode.
                              * As in an endless loop, the ADC constantly converts the analog value present into digital values ​​and returns them. */
    ADCSRA |= (1 << ADEN);    /* Must be set to enable the ADC
                               * ADEN is used to switch the ADC on and off. A 1 at this bit position switches the ADC on.                         */
    ADCSRA |= (1 << ADIE);    /* Enables Interrupt
                               * If a 1 is written to ADIE, the ADC triggers an interrupt after a measurement is completed.
                               * Interrupt is used to get the analog input and in the Interrupt we calculate the adc value by reading out the corresponding registers (ADCH and ADCL)  */
    ADCSRA |= (1 << ADSC);    // Starts the ADC conversion
}

//get method to return the current adc value
int get_adc_current(){
	return adc_current_value;
}

//Interrupt for the ADC
ISR (ADC_vect)
{
    //When a conversion is complete, the converted value is in these two registers (ADCL and ADCH)
	int ADCval = ADCL; //Integer to store the value from register ADCL
	ADCval = (ADCH << 8) + ADCval; /* To read the value of the ADCH register plus the value of ADCL
                                    * Since the result of the ADC is a 10-bit value, this value naturally does not fit into a single register, which is only 8 bits wide.
	                                * Therefore the result is stored in 2 registers ADCL and ADCH.
                                    * By default, the least significant 8 of the 10 result bits are stored in register ADCL and the 2 bits still missing
	                                * are stored in register ADCH at the least significant bit positions. */

	adc_current_value = ADCval; //set adc_current_value to the read adc value
}