/*
 * Ibno055_main.h
 *
 * Created: 08.06.2020 22:07:23
 *  Author: nicok
 */ 
#include <stdint.h>
#include <stdbool.h>

#ifndef IBNO055_MAIN_H_
#define IBNO055_MAIN_H_

void bno_init(void);

void bno055_main(bool debugMode);

typedef struct bnodata{
	char *Pitch;
	
	char *accelForwards;
	char *accelSideways;
	
	char *temperature;
} bnodata;

void getBNOData(bnodata *bnodataPtr);

#endif /* IBNO055_MAIN_H_ */