#include "stuaa.h"
#include "bbia.h"

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int main (int argc, char * argv[]) {
	bbia * a = NULL;
	bbia * b = NULL;
	bbia * c = NULL;

	puts ("BBIA Bitflag tests : \n");
	puts ("Before bitflag :");
	a = bbia_new ();
	bbia_print_levelValue (a);

	puts ("After bitflag set :");
	bbia_bits_flag_set (a, 127);
	bbia_print_levelValue (a);

	puts ("After bitflag unset :");
	bbia_bits_flag_unset (a, 127);
	bbia_print_levelValue (a);

	printf ("Bitshift tests : \n");
	bbia_set_value (a, 0);
	for (int lvl = BBIA_LEVEL_TOP; lvl >= 2; lvl--)
		bbia_at_set (a, lvl, (stuaa_bitflag(32) | stuaa_bitflag(31) ));
	printf ("Before left shift test by 2 : \n");
	bbia_print_levelValue (a);

	bbia_bits_shift_left (a, 2);
	puts ("After left shift by 2 :");
	bbia_print_levelValue (a);

	bbia_set_value (a, 0);
	puts ("Before right shift by 3 :");
	for (int lvl = BBIA_LEVEL_TOP; lvl >= 1; lvl--)
		bbia_at_set (a, lvl, (stuaa_bitflag(1) | stuaa_bitflag(2)
	| stuaa_bitflag(3) | stuaa_bitflag(4) ));
	bbia_print_levelValue (a);

	bbia_bits_shift_right (a, 3);
	puts ("After right shift by 3 :");
	bbia_print_levelValue (a);

	return EXIT_SUCCESS;
}
