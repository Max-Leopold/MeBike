/*
 * hall_main.c
 *
 * Created: 12.06.2020 16:26:52
 *  Author: Wanja Kuch
 */ 


#include "hall_main.h"
#include "adcHall/adcHall.h"
#include "../uart/serial.h"
#include "../util/util.h"

void hall_main() {
	int adcout = getAdcOut();
	char adcOutBuffer[4];
	convertIntToString(adcout, 4, adcOutBuffer);
	serial_print_line(adcOutBuffer);
}

