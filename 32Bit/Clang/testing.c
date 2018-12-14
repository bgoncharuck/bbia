#include "stuaa.h"
#include "bbia.h"

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

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
	bbia_set_zero (a);
	bbia_set_value (a, (stuaa_bitflag(32) | stuaa_bitflag(31) ));
	printf ("Before left shift test by 2 : \n");
	bbia_print_levelValue (a);

	bbia_bits_shift_left (a, 2);
	puts ("After left shift by 2 :");
	bbia_print_levelValue (a);

	bbia_set_zero (a);
	puts ("Before right shift by 3 :");
	bbia_set_value (a, (stuaa_bitflag(1) | stuaa_bitflag(2)
	| stuaa_bitflag(3) | stuaa_bitflag(4) ));
	bbia_print_levelValue (a);

	bbia_bits_shift_right (a, 3);
	puts ("After right shift by 3 :");
	bbia_print_levelValue (a);

	printf ("Compare and check tests : \n");
	bbia_set_zero (a);
	bbia_at_set (a, 3, 42);
	b = bbia_new_fromSystemInteger(42, 0);
	// bbia_at_set (b, 3, 42);
	puts ("a: ");
	bbia_print_levelValue (a);
	puts ("b: ");
	bbia_print_levelValue (b);

	printf("a is %s then b\n", (bbia_compare_bbia(a,b) == 1) ? "bigger" : "lesser");

	c = bbia_new_fromSystemInteger (481, 0);
	printf ("Bbia add bbia tests : \n");
	puts ("b: ");
	bbia_print_levelValue_dec (b);
	puts ("c: ");
	bbia_print_levelValue_dec (c);
	bbia_add_bbia (b,c);
	printf("b + c = \n");
	bbia_print_levelValue_dec (b);

	printf ("Bbia dif bbia tests : \n");
	puts ("b: ");
	bbia_print_levelValue_dec (b);
	puts ("c: ");
	bbia_print_levelValue_dec (c);
	bbia_sub_bbia (b,c);
	printf("b - c = \n");
	bbia_print_levelValue_dec (b);
	bbia_set_zero (c);
	bbia_at_set (c, BBIA_LEVEL_TOP, 44);
	printf("b - 44 = \n");
	bbia_sub_bbia (b,c);
	bbia_print_levelValue_dec (b);
	bbia_set_zero (c);
	bbia_at_set (c, BBIA_LEVEL_TOP, 5);
	printf("b + 5 = \n");
	bbia_add_bbia (b,c);
	bbia_print_levelValue_dec (b);
	bbia_set_zero (c);
	printf ("Bbia mult and div tests : \n");
	bbia_at_set (c, BBIA_LEVEL_TOP, 111);
	puts ("c: ");
	bbia_print_levelValue_dec (c);
	bbia_mult_bbia (c,b);
	puts ("c * 3: ");
	bbia_print_levelValue_dec (c);
	puts ("c / 111: ");
	bbia_div_int (c, 111);
	bbia_print_levelValue_dec (c);
	bbia_at_set (c, BBIA_LEVEL_TOP, 3);
	bbia_at_set (c, BBIA_LEVEL_TOP-1, 7);
	char * someStr = bbia_base_to (c, 16);
	printf("%lu == %s\n", strlen(someStr), someStr);

	bbia * d = bbia_base_from (someStr, 16);
	free(someStr);
	bbia_print_levelValue_dec (d);

	bbia * e = bbia_new_fromSystemInteger (256,0);
	bbia_log_int (2,0,e);
	bbia_print_levelValue_dec (e);

	if (a != NULL) bbia_free (a);
	if (b != NULL) bbia_free (b);
	if (c != NULL) bbia_free (c);
	if (d != NULL) bbia_free (d);
	if (e != NULL) bbia_free (e);
	return EXIT_SUCCESS;
}
