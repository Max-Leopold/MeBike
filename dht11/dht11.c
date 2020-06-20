/*
 * dht11.c
 *
 * 
 *  Author: Frederic Henn
 */
#include <avr/io.h>
#include "../util/util.h"
#include "dht11.h"
#include "../uart/serial.h"


uint8_t Integer_Humidity, Decimal_RH, Integer_Temp, Decimal_Temp;

uint8_t received_data = 0;


void Request()                /* Microcontroller send start pulse/request */
{
    DDRD |= (1 << DHT11_PIN);
	 PORTD &= ~(1<<DHT11_PIN);	/* set to low pin */
	 PORTD |= (1<<DHT11_PIN);	/* set to high pin */
 }

 /* Response method needs to be done, because after start signal (Request-method) the dht11-sensor sends a response signal
  * Further information on this website: https://components101.com/dht11-temperature-sensor
  * And on the datasheet: https://components101.com/sites/default/files/component_datasheet/DHT11-Temperature-Sensor.pdf*/
 void Response()				/* receive response from DHT11 */
 {
	 DDRD &= ~(1<<DHT11_PIN);
	 while(PIND & (1<<DHT11_PIN));
	 while((PIND & (1<<DHT11_PIN))==0);
	 while(PIND & (1<<DHT11_PIN));
 }

 uint8_t Receive_data()			/* receive data */
 {
	 for (int q=0; q<8; q++)
	 {
		 while((PIND & (1<<DHT11_PIN)) == 0);  /* check received bit 0 or 1 */
		 if(PIND & (1<<DHT11_PIN))/* if high pulse is greater than 30ms */
         received_data = (received_data<<1)|(0x01);	/* then its logic HIGH */
		 else			/* otherwise its logic LOW */
         received_data = (received_data<<1);
		 while(PIND & (1<<DHT11_PIN));
	 }
	 return received_data;
 }

 void dht11_main() 
 {

		Request();		/* send start pulse */
		Response();		/* receive response */

		// We get a 40-bit response from the sensor and in I_Temp we get the relevant part of this response - the temperature
        Integer_Humidity=Receive_data();	/* store first eight bit in Integer_Humidity */
     Decimal_RH = Receive_data();    /* store next eight bit in Decimal_RH */
     Integer_Temp = Receive_data();    /* store next eight bit in Integer_Temp */
     Decimal_Temp = Receive_data();    /* store next eight bit in Decimal_Temp */

     int length = 0;

     if (Integer_Temp < 10 && Integer_Temp > -10) {
         length = 2;
     }
     char tempData[length];
     convertIntToString(Integer_Temp, length, tempData);
     serial_print_line(tempData);

     struct temperature_value tData;
     tData.temperature = &tempData[0];

     //TO-DO - send via Bluetooth
 }
