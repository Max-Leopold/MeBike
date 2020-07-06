//
// Created by Max Leopold on 06/06/2020.
//

#include "main.h"
#include "uart/serial.h"
#include "gps/gps_main.h"
#include "hall/hall_main.h"
#include "bluetooth/bluetooth.h"
#include "adc/adc.h"
#include "pulsesensor/pulsesensor.h"
#include "util/Interrupt/timer.h"
#include "BNO055/bno055_main.h"
#include <util/delay.h>
#include <avr/interrupt.h>

char debugMode = '0';

void init() {
	initMillis();

	// Bluetooth uses the serial class internally, so it does not need to be initialized here
	serial_init();
	bluetooth_init();
	hall_init();
	gps_init();
	bno_init();
	ADC_init();
	pulsesensor_init();

    sei();
}


int main() {

    init();

    while (1) {
        gps_main();
		hall_main();
		pulsesensor_main();
		bno055_main(debugMode);

	}
}