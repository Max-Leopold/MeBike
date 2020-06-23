<<<<<<< HEAD
/*
 * MeBike.c
 *
 * Created: 03.06.2020 16:36:07
 * Author : nicok
 */ 

#include <avr/io.h>


int main(void)
{
    /* Replace with your application code */
    while (1) 
    {
=======
//
// Created by Max Leopold on 06/06/2020.
//

#include "main.h"
#include "uart/serial.h"
#include "gps/gps_main.h"
#include "hall/hall_main.h"
#include "bluetooth/bluetooth.h"
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
		hall_main();
>>>>>>> 64a42f379064dc0d815c2ddb458fccd4629c2f0c
    }
}

