//
// Created by Max Leopold on 06/06/2020.
//

#include "main.h"
#include "uart/serial.h"
#include "gps/gps_main.h"
#include "bluetooth/bluetooth.h"
#include "avr/delay.h"

#include <avr/interrupt.h>

void init() {
	// Bluetooth uses the serial class internally, so it does not need to be initialized here
	serial_init();
	
	bluetooth_init();
	
    sei();
}


int main() {

    init();

    gps_init();

    while (1) {
        gps_main();
		struct gps_coordinates gps;
		gps.gmt_time = "12";
		gps.latitude = "345";
		gps.longitude = "678";
		bluetooth_send_gps(gps);
		_delay_ms(1000);
    }
}

