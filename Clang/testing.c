#include "stuaa.h"
#include "bbia.h"

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int main (int argc, char * argv[]) {

	/*
	bbia * firstTest = bbia_new();

	bbia_at_set (firstTest, BBIA_LEVEL_TOP, BBIA_LEVEL_IS_FULL);

	bbia_print_levelValue (firstTest);

	bbia_free (firstTest);
	*/

	// int secondTest = 0x12345678;
	// int secondTest = BBIA_LEVEL_IS_FULL;
	// int secondTest = INT_MAX;
	// int secondTest = 0;
	int secondTest = -1;


	char * secondTestToBase = stuaa_toBase (secondTest, 2);
	/*
	*/
	for (int i = 0; i < BBIA_INTEGER_SIZE+1; i++)
	putchar ( (secondTestToBase[i] != '\0') ? secondTestToBase[i] : '0' );

	if (secondTestToBase) free(secondTestToBase);

	return EXIT_SUCCESS;
}
