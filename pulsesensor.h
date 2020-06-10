/*
 * pulsesensor.h
 *
 *  Author: Frederic Henn
 */ 

#ifndef PULSESENSOR_H_
#define PULSESENSOR_H_

	struct pulse_value {
		char *pulse;
	};

	int ADCsingleREAD(uint8_t);

	bool heartbeatDetected(int);

	
#endif /* PULSESENSOR_H_ */