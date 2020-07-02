/*
 * hall_main.h
 *
 * Created: 12.06.2020 14:27:04
 *  Author: Wanja
 */ 

#ifndef ME_BIKE_HALL_MAIN_H
#define ME_BIKE_HALL_MAIN_H

void hall_init(void);
void hall_main(void);
void calcHallRoation(void);

struct HALLDATA{
	char *rpm;
	};

#endif //ME_BIKE_HALL_MAIN