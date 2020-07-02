/*
 * Ibno055_main.h
 *
 * Created: 08.06.2020 22:07:23
 *  Author: nicok
 */ 
#include <stdint.h>

#ifndef IBNO055_MAIN_H_
#define IBNO055_MAIN_H_

void bno_init(void);

void bno055_main(void);

struct BNODATA{
	char *Pitch;
	
	char *xAccel;
	char *yAccel;
	};

#endif /* IBNO055_MAIN_H_ */