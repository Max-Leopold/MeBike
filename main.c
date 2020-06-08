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

#include <avr/interrupt.h>

void init() {
    serial_init();
    sei();
}


int main() {

    init();

    gps_init();

    while (1) {
        gps_main();
>>>>>>> 7acf765fdf38fc91389428506c3de9a0eb1edab3
    }
}

