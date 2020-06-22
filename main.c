//
// Created by Max Leopold on 06/06/2020.
//

#include "main.h"
#include "uart/serial.h"
#include "gps/gps_main.h"
#include "bluetooth/bluetooth.h"
#include "adc/adc.h"
#include <avr/interrupt.h>

void init() {
	// Bluetooth uses the serial class internally, so it does not need to be initialized here
	serial_init();
	
	bluetooth_init();

	ADC_init();
	
    sei();
}


int main() {

    init();

    gps_init();

    while (1) {
        gps_main();
    }
}

