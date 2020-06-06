//
// Created by Max Leopold on 06/06/2020.
//

#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#ifndef BAUD
#define BAUD 9600
#endif
#ifndef BAUD_TOL
#define BAUD_TOL 2
#endif

#ifndef AVR_TEST_SERIAL_H
#define AVR_TEST_SERIAL_H

void serial_print_line(char *str);

void serial_print(char *str);

void serial_write(char ch);

void serial_init(void);

#endif //AVR_TEST_SERIAL_H
