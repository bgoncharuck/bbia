#include "stuaa.h"
#include "bbia.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <math.h>

#define throw(MSG) fprintf(stderr, "%s\n", MSG)

static const char * numerics =
"0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

static inline double log_base(float base, float num) {
	return log2(num) / log2(base);
}

char * stuaa_toBase (int sinteger, int base) {

	if ( !(base < 65 && base > 1) ) {
		throw ("The base must be from 2 to 64");
		return NULL;
	}

	// TODO Signed To Unsigned Arithmetic Algorithm
	unsigned integer = sinteger;

	char * result = malloc (sizeof(char) * BBIA_INTEGER_SIZE + 1);
	if (result == NULL) abort();

	int start = ceil ( log_base (base, integer) ) - 1;
	int currentDigit = BBIA_INTEGER_SIZE-1;

	while(integer){
		result[start] =
		numerics [ integer % base ];

		integer /= base;

		currentDigit--;
		start--;
	}

	return result;
}

int stuaa_fromBase (char * integer, int base) {

	return -1;
}
