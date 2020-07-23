/*
 adcHall.c
 
 Created: 22.06.2020 22:23:08
 Author: Wanja
 
 ported from cpp library from Uros Petrevski (see here: https://github.com/nodesign/MCP3008/blob/master/MCP3008.cpp)
 which is ported from Python code originaly written by Adafruit learning system for rPI :
 http://learn.adafruit.com/send-raspberry-pi-data-to-cosm/python-script
 
 */ 

#include <avr/io.h>
#include <stdint.h>
#include <stdio.h>

int getAdcOut(){
	//clock = 9 => PB1
	//miso = 10 => PB2
	//mosi = 11 => PB3
	//cspin = 12 => PB4
	DDRB |= (0 << DDB2);
	DDRB |= (1<< DDB4);
	PORTB |= (1 << PORTB4);
	DDRB |= (1<< DDB3);
	DDRD |= (1<< DDD7);
	PORTD &= ~(1 << PORTD7);
	PORTB &= ~(1 << PORTB4);
	int commandout = 0;
	commandout |= 0x18; //  # start bit + single-ended bit
	commandout <<= 3; //    # we only need to send 5 bits here

	for (int i=0; i<5; i++) {
		if (commandout & 0x80){
			PORTB |= (1 << PORTB3);
			}else{
			PORTB &= ~(1 << PORTB3);
		}
		commandout <<= 1;
		PORTD |= (1 << PORTD7);
		PORTD &= ~(1 << PORTD7);
	}

	int adcout = 0;
	// read in one empty bit, one null bit and 10 ADC bits
	for (int i=0; i<12; i++) {
		PORTD |= (1 << PORTD7);
		PORTD &= ~(1 << PORTD7);
		adcout <<= 1;
		if(PINB & (1 << DDB2)){
			adcout |= 0x1;
		}
	}
	PORTB |= (1 << PORTB4);
	adcout >>= 1; //      # first bit is 'null' so drop it
	return adcout;
}