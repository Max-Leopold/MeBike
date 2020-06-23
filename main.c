//
// Created by Max Leopold on 06/06/2020.
//

#include "main.h"
#include "uart/serial.h"
#include "gps/gps_main.h"
#include <util/delay.h>


#include <avr/interrupt.h>

void init() {
    serial_init();
    sei();
}


int main() {

    init();
    gps_init();
	bno_init();

    while (1) {
        gps_main();
		bno055_main();
		_delay_ms(500);
    }
}


