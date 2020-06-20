/*
 * dht11.c
 *
 * 
 *  Author: Frederic Henn
 */ 
 #define F_CPU 16000000UL
 #include <avr/io.h>
 #include <stdio.h>
 #include <util/delay.h>
 #include <stdlib.h>
 #include "dht11.h" 
 #define DHT11_PIN 6

 uint8_t I_RH,D_RH,I_Temp,D_Temp,CheckSum;

 uint8_t c=0;


 void Request()				/* Microcontroller send start pulse/request */
 {
	 DDRD |= (1<<DHT11_PIN);
	 PORTD &= ~(1<<DHT11_PIN);	/* set to low pin */
	 _delay_ms(20);			/* wait for 20ms */
	 PORTD |= (1<<DHT11_PIN);	/* set to high pin */
 }

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
		 _delay_us(30);
		 if(PIND & (1<<DHT11_PIN))/* if high pulse is greater than 30ms */
		 c = (c<<1)|(0x01);	/* then its logic HIGH */
		 else			/* otherwise its logic LOW */
		 c = (c<<1);
		 while(PIND & (1<<DHT11_PIN));
	 }
	 return c;
 }

 void dht11_main() 
 {

		Request();		/* send start pulse */
		Response();		/* receive response */

		// We get a 40-bit response from the sensor and in I_Temp we get the relevant part of this response - the temperature
		I_RH=Receive_data();	/* store first eight bit in I_RH */
		D_RH=Receive_data();	/* store next eight bit in D_RH */
		I_Temp=Receive_data();	/* store next eight bit in I_Temp */
		D_Temp=Receive_data();	/* store next eight bit in D_Temp */
		CheckSum=Receive_data();/* store next eight bit in CheckSum */

		// For testing purpose
		char tempData[1];
		sprintf(tempData, "%d", I_Temp);
		
		/*
		serial_print("Temperatur: ");
		serial_print_line(tempData);
		_delay_ms(1000);
		*/
		

		struct temperature_value tData;
		tData.temperature = tempData[0];

		//TO-DO - send via Bluetooth


 }
