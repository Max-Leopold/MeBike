//
// Created by Max Leopold on 06/06/2020.
//
#ifndef
#define F_CPU 16000000UL
#endif
#include <avr/interrupt.h>
#include <stdbool.h>
#include <stdio.h>
#include <avr/io.h>
#include "uart/serial.h"
#include "dht11.h"
#include "main.h"
#include "uart/serial.h"
#include "gps/gps_main.h"
#include "bluetooth/bluetooth.h"


void init() {
	// Bluetooth uses the serial class internally, so it does not need to be initialized here
	serial_init();

	bluetooth_init();

    sei();
}


int main() {

    init();

    gps_init();

    ADC_init();

    while (1) {
        gps_main();

         dht11_main();

        //TO-DO - send via Bluetooth
    }
}
