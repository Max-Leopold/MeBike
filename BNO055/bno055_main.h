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

struct BNOGYRO{
	//tilt area
	int16_t eulerPitch;
	};
	
struct BNOACCEL{
	char *xAccel;
	};

#endif /* IBNO055_MAIN_H_ */