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

    UCSR0C = 0x06;       /* Set frame format: 8data, 1stop bit  */
    UCSR0B = (1 << TXEN0); /* Enable  transmitter                 */
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
