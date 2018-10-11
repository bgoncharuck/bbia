#include "stuaa.h"
#include "bbia.h"

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int main (int argc, char * argv[]) {

	/*
	*/
	bbia * firstTest = bbia_new();

	// bbia_at_set (firstTest, BBIA_LEVEL_TOP, BBIA_LEVEL_IS_FULL);
	// bbia_set_value (firstTest, -1);
	for (int lvl = BBIA_LEVEL_TOP; lvl >= 0; lvl--)
		bbia_at_set (firstTest, lvl, (stuaa_bitflag(32) | stuaa_bitflag(31) ));
		// bbia_at_set (firstTest, lvl, (stuaa_bitflag(31) ));

	bbia_print_levelValue (firstTest);

	bbia_bitshift_left (firstTest, 2);
	bbia_print_levelValue (firstTest);

	for (int lvl = BBIA_LEVEL_TOP; lvl >= 0; lvl--)
		bbia_at_set (firstTest, lvl, (stuaa_bitflag(1) | stuaa_bitflag(2)
	| stuaa_bitflag(3) | stuaa_bitflag(4) ));
	bbia_print_levelValue (firstTest);

	bbia_bitshift_right (firstTest, 3);
	bbia_print_levelValue (firstTest);


	bbia_free (firstTest);

	return EXIT_SUCCESS;
}
