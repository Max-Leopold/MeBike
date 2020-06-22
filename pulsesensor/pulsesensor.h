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

char heartbeatDetected(int, int);

int getBpm();

void addValue(int);

void clearBpm();

void pulsesensor_main();

#endif /* PULSESENSOR_H_ */