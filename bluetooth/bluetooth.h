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
void bluetooth_send_gps(char *gmt_time, char *latitude, char *longitude);

/************************************************************************/
/* Expects the temperature coordinates struct                           */
/************************************************************************/
void bluetooth_send_temperature(char *temperature);

/************************************************************************/
/* Expects the pulse coordinates struct                                 */
/************************************************************************/
void bluetooth_send_pulse(char *pulse);

/************************************************************************/
/* Expects the gyro coordinates struct                                  */
/************************************************************************/
void bluetooth_send_gyro(char *accFwd, char *accSd, char *pitch);

/************************************************************************/
/* Expects the rpm values			                                    */
/************************************************************************/
void bluetooth_send_rpm(char *rpm);



#endif /* BLUETOOTH_H_ */