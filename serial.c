//
// Created by Max Leopold on 06/06/2020.
//

#include "serial.h"
#include <util/setbaud.h>
#include <avr/io.h>

void serial_init() {
	// set UART0 baud rate
	UBRR0H = UBRRH_VALUE;
	UBRR0L = UBRRL_VALUE;

	UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00); //Set frame format: 8 bit data, 1 stop bit
	UCSR0B |= (1 << TXEN0); //enable TX
	UCSR0B |= (1 << RXEN0); //enable RX
	UCSR0B |= (1 << RXCIE0); //RX complete interrupt
}

void serial_print_line(char *str) {
	serial_print(str);
	serial_write(0x0d);
	serial_write(0x0a);
}

void serial_print(char *str) {
	while (*str) {
		serial_write(*str++);
	}
}

void serial_write(char ch) {
	while (!(UCSR0A & (1 << UDRE0))) {

	}
	UDR0 = ch;
}
