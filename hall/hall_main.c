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
#include "../util/Interrupt/timer.h"

int changesSinceReset;
char lastSignal;
unsigned long startMillis;

char *rotationsPerMinute;

void hall_init(){
	startMillis = getMillis();
}

void hall_main() {
	
	calcHallRoation();
	
	int adcout = getAdcOut();
	char currentSignal = 0;
	if(adcout > 50){
		currentSignal = 1;	
	}
	if(currentSignal != lastSignal){
		changesSinceReset++;
	}
	lastSignal = currentSignal;
}

void calcHallRoation(){
	int millisElapsed = getMillis() - startMillis;
	if(millisElapsed > 1000){
		startMillis = getMillis();
		char signalBuffer[4];
		//changesSinceReset*5 because its called every second => times 60
		//then divided by 12 because the wheel has 12 magnets => each signal is 1/12th of a rotation
		convertIntToString((changesSinceReset*5), 4, signalBuffer);
		
		//here is the result 
		rotationsPerMinute = signalBuffer;	
		
		changesSinceReset = 0;
	}
}