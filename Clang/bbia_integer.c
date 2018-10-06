#include "bbia_integer.h"
#include "bbia.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define throw(MSG) fprintf(stderr, "%s\n",MSG)

static const char * numerics =
"0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ/-+=_!@#$";

static inline double log_base(float base, float num) {
	return log(num) / log(base);
}

char * bbia_integer_toBase (int integer, int base) {

	if ( !(base < strlen(numerics) && base > 1) ) {
		throw ("The base must be from 2 to 64");
		return NULL;
	}

	char * result = malloc (sizeof(char) * BBIA_INTEGER_SIZE + 1);
	if (result == NULL) abort();

	// int start = (base > 10) ? ceil ( log_base (base, integer) ) - 1 ;
	int start = ceil ( log_base (base, integer) ) - 1;
	int currentDigit = BBIA_INTEGER_SIZE-1;

	printf("%i\n", start);

	if (base == 10)
		sprintf (result, "%i", integer);

	else {
		if (base < 10)
			while(integer){
				sprintf(&result[start], "%i", integer % base);
				integer /= base;
				currentDigit--;
				start--;

		}

		else if (base > 10)
			while(integer){
				result[start] =
				numerics[(integer % base)];
				integer /= base;
				currentDigit--;
				start--;
		}
	}

	for (int i = 0; i < BBIA_INTEGER_SIZE+1; i++)
	putchar ( (result[i] != '\0') ? result[i] : 'N' );

	return result;
}

int bbia_integer_fromBase (char * integer, int base) {

	return -1;
}
