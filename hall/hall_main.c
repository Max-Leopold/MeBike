/*
 * hall_main.c
 *
 * Created: 12.06.2020 16:26:52
 *  Author: Wanja Kuch
 */ 


#include "hall_main.h"
#include "adcHall/adcHall.h"
#include <stdio.h>
#include <string.h>
#include "../uart/serial.h"
#include "../util/util.h"
#include "../util/Interrupt/timer.h"
#include "../bluetooth/bluetooth.h"

int changesSinceReset;
char lastSignal;
unsigned long hallStartMillis;

char *rotationsPerMinute;

void hall_init(){
	hallStartMillis = getMillis();		//timer is needed in order to calculate rotations per minute and to time the sending and calculation of data once a second
}

void hall_main() {	
	int adcout = getAdcOut();	//get signal from ADC => digital signal converted from the analog signal coming from the hall sensor (range = 0 - 1023) 
								//high signal = magnet near the sensor
	char currentSignal = 0;
	if(adcout > 50){			//no signal => around 0, typical on signal has strength of 100 => threshold of 50
		currentSignal = 1;		//currentSignal = 1 => magnet near the sensor
	}
	if(currentSignal != lastSignal && currentSignal == 1){	//comparison with last signal so a magnet passing the sensor is only registered once
		changesSinceReset++;			//count magnets passing the sensor
	}
	lastSignal = currentSignal;			//save current signal for next function call
	
	calcHallRoation();		//calculate the rotations per minute and send send to app
}

void calcHallRoation(){
	int millisElapsed = getMillis() - hallStartMillis;	//calculate time since last calculation
	if(millisElapsed > 1000){							//calculate and send only once a second
		hallStartMillis = getMillis();	//reset timer
		char signalBuffer[4]; //char buffer for int to string conversion
		//changesSinceReset*5 because its called every second => times 60
		//then divided by 12 because the wheel has 12 magnets => each signal is 1/12th of a rotation
		sprintf(signalBuffer, "%d", (changesSinceReset*5)); //convert int to string
		rotationsPerMinute = signalBuffer;					//variable added for better readability of code
		
		changesSinceReset = 0; //reset the signal counter
			
		bluetooth_send_rpm(rotationsPerMinute);	//send to app via bluetooth
	}
	
}