/*
 * bluetooth.h
 *
 * Created: 10.06.2020 10:58:38
 *  Author: Flo
 */ 

#include "../gps/gps_main.h"

#ifndef BLUETOOTH_H_
#define BLUETOOTH_H_

#define DATA_DELIMITER "|"
#define MESSAGE_BEGIN "<BOM>"
#define MESSAGE_END "<EOM>"


/************************************************************************/
/* Initializes the bluetooth parameters need to send data               */
/************************************************************************/
void bluetooth_init(void);

/************************************************************************/
/* Expects the gps coordinates struct                                   */
/************************************************************************/
void bluetooth_send_gps(struct gps_coordinates);

/************************************************************************/
/* Expects the temperature coordinates struct                           */
/************************************************************************/
void bluetooth_send_temperature(void);

/************************************************************************/
/* Expects the pulse coordinates struct                                 */
/************************************************************************/
void bluetooth_send_pulse(void);

/************************************************************************/
/* Expects the gyro coordinates struct                                  */
/************************************************************************/
void bluetooth_send_gyro(void);



#endif /* BLUETOOTH_H_ */