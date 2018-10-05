#include "bbia.h"

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int main (int argc, char * argv[]) {

	bbia * firstTest = bbia_new();

	bbia_at_set (firstTest, LEVEL_TOP, INT_MAX);

	bbia_print_levelValue (firstTest);

	bbia_free (firstTest);

	return EXIT_SUCCESS;
}
