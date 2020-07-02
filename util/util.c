//
// Created by Max Leopold on 22/06/2020.
//

#include "util.h"

/**
 *
 * @param number; number to be converted to a string
 * @param maxIntLength; the max length the number can be. e.g. max num is 999 -> maxIntLength is 3
 * @param buffer; char array which has the same length as the maxIntLength
 * @return
 */
char *convertIntToString(int number, int maxIntLength, char *buffer) {

    buffer[maxIntLength] = '\0';
    for (int i = maxIntLength - 1; i >= 0; --i) {
        buffer[i] = (number % 10) + '0';
        number = number / 10;
    }

    return buffer;
}

char *convertDoubleToString(double val, signed char width, unsigned char prec, char *sout){
	asm(".global _printf_float");

	char fmt[20];
	sprintf(fmt, "%%%d.%df", width, prec);
	sprintf(sout, fmt, val);
	return sout;
}