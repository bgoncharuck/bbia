#include "stuaa.h"
#include "bbia.h"

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int main (int argc, char * argv[]) {

	bbia * firstTest = bbia_new();
	bbia * secondTest = bbia_new();

	puts ("BBIA Bitflag tests : \n");
	puts ("Before bitflag :");
	bbia_print_levelValue (secondTest);

	puts ("After bitflag set :");
	bbia_bitflag_set (secondTest, 127);
	bbia_print_levelValue (secondTest);

	puts ("After bitflag unset :");
	bbia_bitflag_unset (secondTest, 127);
	bbia_print_levelValue (secondTest);

	puts ("\nShift tests :\n");
	// bbia_at_set (firstTest, BBIA_LEVEL_TOP, BBIA_LEVEL_IS_FULL);
	// bbia_set_value (firstTest, -1);
	for (int lvl = BBIA_LEVEL_TOP; lvl >= 0; lvl--)
		bbia_at_set (firstTest, lvl, (stuaa_bitflag(32) | stuaa_bitflag(31) ));
		// bbia_at_set (firstTest, lvl, (stuaa_bitflag(31) ));

	puts ("Before left shift by 2 :");
	bbia_print_levelValue (firstTest);

	bbia_bitshift_left (firstTest, 2);
	puts ("After left shift by 2 :");
	bbia_print_levelValue (firstTest);

	for (int lvl = BBIA_LEVEL_TOP; lvl >= 0; lvl--)
		bbia_at_set (firstTest, lvl, (stuaa_bitflag(1) | stuaa_bitflag(2)
	| stuaa_bitflag(3) | stuaa_bitflag(4) ));
	puts ("Before right shift by 3 :");
	bbia_print_levelValue (firstTest);

	bbia_bitshift_right (firstTest, 3);
	puts ("After right shift by 3 :");
	bbia_print_levelValue (firstTest);

	/*
	int shiftTest = 0x40000000;
	// int shiftTest1 = -1;
	// unsigned int shiftTest = shiftTest1;
	shiftTest >>= 3;
	printf("\n%d\n", shiftTest);
	puts(stuaa_toBase(shiftTest,2));
	*/

	bbia_free (secondTest);
	bbia_free (firstTest);

	return EXIT_SUCCESS;
}
