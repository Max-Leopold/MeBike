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

	void ADC_init();

	bool heartbeatDetected(int,int);

	int getBpm();

	void addValue(int);

	void clearBpm();
	
#endif /* PULSESENSOR_H_ */