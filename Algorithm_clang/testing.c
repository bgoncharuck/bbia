#include "stuaa.h"
#include "bbia.h"

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int main (int argc, char * argv[]) {

	bbia * bitShiftTest = bbia_new();
	bbia * bitFlagTest = bbia_new();
	bbia * sumTest = bbia_new();
	bbia * multTest = bbia_new();
	bbia * multB1Test = bbia_new();
	bbia_at_set (multB1Test,BBIA_LEVEL_TOP, 2);
	// bbia * multB2Test = bbia_new();
	bbia * multB2Test = NULL;
	bbia * multB3Test = bbia_new();
	bbia * divisionTest1 = bbia_new();


	puts ("BBIA Bitflag tests : \n");
	puts ("Before bitflag :");
	bbia_print_levelValue (bitFlagTest);

	puts ("After bitflag set :");
	bbia_bits_flag_set (bitFlagTest, 127);
	bbia_print_levelValue (bitFlagTest);

	puts ("After bitflag unset :");
	bbia_bits_flag_unset (bitFlagTest, 127);
	bbia_print_levelValue (bitFlagTest);

	puts ("\nShift tests :\n");
	// bbia_at_set (bitShiftTest, BBIA_LEVEL_TOP, BBIA_LEVEL_IS_FULL);
	// bbia_set_value (bitShiftTest, -1);
	for (int lvl = BBIA_LEVEL_TOP; lvl >= 0; lvl--)
		bbia_at_set (bitShiftTest, lvl, (stuaa_bitflag(32) | stuaa_bitflag(31) ));
		// bbia_at_set (bitShiftTest, lvl, (stuaa_bitflag(31) ));

	puts ("Before left shift by 2 :");
	bbia_print_levelValue (bitShiftTest);

	bbia_bits_shift_left (bitShiftTest, 2);
	puts ("After left shift by 2 :");
	bbia_print_levelValue (bitShiftTest);

	for (int lvl = BBIA_LEVEL_TOP; lvl >= 0; lvl--)
		bbia_at_set (bitShiftTest, lvl, (stuaa_bitflag(1) | stuaa_bitflag(2)
	| stuaa_bitflag(3) | stuaa_bitflag(4) ));
	puts ("Before right shift by 3 :");
	bbia_print_levelValue (bitShiftTest);

	bbia_bits_shift_right (bitShiftTest, 3);
	puts ("After right shift by 3 :");
	bbia_print_levelValue (bitShiftTest);

	puts("\nSum && sub tests\n");
	puts("Before sum test :");
	bbia_print_levelValue_dec (sumTest);
	// bbia_sum_int (sumTest, 100);
	// bbia_sum_int_level (sumTest, INT_MAX, BBIA_LEVEL_TOP - 1);
	// bbia_sum_int_level (sumTest, BBIA_LEVEL_IS_FULL, BBIA_LEVEL_TOP - 1);
	puts("After sum test :");

	for (int i = 1; i < BBIA_LEVEL_TOP - 2; i++)
		bbia_sum_int_level (sumTest, BBIA_LEVEL_IS_FULL, i);

	bbia_sum_int_level (sumTest, BBIA_LEVEL_IS_PFULL, BBIA_LEVEL_TOP);
	bbia_sum_int_level (sumTest, 450, BBIA_LEVEL_TOP-1);
	bbia_sum_int_level (sumTest, BBIA_LEVEL_IS_FULL, BBIA_LEVEL_TOP-2);

	bbia_print_levelValue_dec (sumTest);

	puts("After sum test second :");

	bbia_sum_int_level (sumTest, 7, BBIA_LEVEL_TOP-2);
	bbia_print_levelValue_dec (sumTest);

	puts("\nMult tests\n");
	puts("Before mult test : ");
	bbia_at_set (multTest, BBIA_LEVEL_TOP, 14884245);
	bbia_print_levelValue_dec (multTest);

	puts("After mult test (x 3): ");

	bbia_mult_int (multTest, 3);
	bbia_print_levelValue_dec (multTest);

	puts("Before pow(5) test : ");
	bbia_at_set (multB3Test, BBIA_LEVEL_TOP, 2);
	bbia_print_levelValue_dec (multB3Test);

	puts("After pow(5) test : ");
	// multB2Test = bbia_mult_bbia_new (multB3Test, multB3Test);
	// bbia_mult_bbia (multB3Test, multB3Test);
	bbia_pow (multB3Test, 5);
	bbia_print_levelValue_dec (multB3Test);

	puts("\nDivsion tests\n");
	puts("Before division test : ");
	bbia_at_set (multTest, BBIA_LEVEL_TOP, 56);
	bbia_print_levelValue_dec (divisionTest1);



	/*
	int shiftTest = 0x40000000;
	// int shiftTest1 = -1;
	// unsigned int shiftTest = shiftTest1;
	shiftTest >>= 3;
	printf("\n%d\n", shiftTest);
	puts(stuaa_toBase(shiftTest,2));
	*/

	bbia_free (divisionTest1);
	bbia_free (multB1Test);
	// bbia_free (multB2Test);
	bbia_free (multB3Test);
	bbia_free (multTest);
	bbia_free (sumTest);
	bbia_free (bitFlagTest);
	bbia_free (bitShiftTest);

	return EXIT_SUCCESS;
}
