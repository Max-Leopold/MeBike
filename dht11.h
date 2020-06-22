/*
 * dht11.h
 *
 *  Author: Frederic Henn
 */

#ifndef DHT11_H_
#define DHT11_H_
#define DHT11_PIN 6

	struct temperature_value {
		char *temperature;
	};

    void Request();
    void Response();
    uint8_t Receive_data();
    void dht11_main();

#endif /* DHT11_H_ */
