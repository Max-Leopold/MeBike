/*
 * Me-Bike.c
 *
 * Created: 03.06.2020 17:15:55
 * Author : nicok
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

const int delayMsec = 60;

int main(void)
{
	serial_init();

	static int beatMsec = 0;
	int heartRateBPM = 0;


	while (1)
	{
		 if(heartbeatDetected(delayMsec))
		 {
			heartRateBPM = 60000 / beatMsec;

			char buffer[16];
			sprintf(buffer, "%d", heartRateBPM);
			serial_print("Puls: ");
			serial_print_line(buffer);
			beatMsec = 0;
		 }
		 _delay_ms(delayMsec);
		 beatMsec += delayMsec;

	}
}

