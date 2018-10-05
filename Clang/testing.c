#include "bbia.h"

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int main (int argc, char * argv[]) {

	bbia * firstTest = bbia_new();

	bbia_at_set (firstTest, BBIA_LEVEL_TOP, BBIA_LEVEL_IS_FULL);

	bbia_print_levelValue (firstTest);

	bbia_free (firstTest);

	return EXIT_SUCCESS;
}