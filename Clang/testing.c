#include "bbia_integer.h"
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
	int secondTest = INT_MAX;


	char * secondTestToBase = bbia_integer_toBase (secondTest, 16);
	printf("%s\n", secondTestToBase);

	if (secondTestToBase) free(secondTestToBase);

	return EXIT_SUCCESS;
}
