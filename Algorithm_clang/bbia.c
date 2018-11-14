#include "stuaa.h"
#include "bbia.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define throw(MSG) fprintf(stderr, "%s\n",MSG)

typedef enum __DIVISION_MOD {
	DIVISION = 42,
	MOD = 43
} DIVISION_MOD;

static const char * numerics =
"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz+/,"; //base64

static inline int log_formMax () {
	// log2(2^BBIA_INTEGER_SIZE * BBIA_LEVEL_COUNT) / log2(2) ;
	// equal to BBIA_INTEGER_SIZE * BBIA_LEVEL_COUNT
	return BBIA_INTEGER_SIZE * BBIA_LEVEL_COUNT;
}

struct __bbia {

	// some languages don`t have unsigned int
	signed int at[BBIA_LEVEL_COUNT];
	signed int sign;
};

// @BITSHIFT

void bbia_bits_shift_left (bbia * self, int value) {

	if (self == NULL) {
		throw("null pointer in bbia_bits_shift_left()");
		return;
	}

	int savedBits [BBIA_LEVEL_TOP-1];

	int bitMask[2] = {0,0};
	for (int curBit = BBIA_INTEGER_SIZE-value+1; curBit <= BBIA_INTEGER_SIZE; curBit++)
		bitMask[0] |= stuaa_bitflag(curBit);

	// 1. Shift with saved bits
	// we shift all levels from top to zero
	// but save the part which is lost
	// zero level not count in saving
	// we saved bits in position INTEGER_SIZE...INTEGER_SIZE-value
	// but need set bits in position value...1

	// 2. Set saved bits
	// for all levels lesser then top
	// we set saved bits

	for (int lvl = BBIA_LEVEL_TOP; lvl > 0; self->at[lvl] <<= value, lvl--) {
		bitMask[1] = self->at[lvl] & bitMask[0];
		stuaa_shiftr (bitMask+1, BBIA_INTEGER_SIZE-value);
		savedBits[lvl-1] = bitMask[1];
	}
	self->at[0] <<= value;

	for (int lvl = 0; lvl < BBIA_LEVEL_TOP; lvl++)
		self->at[lvl] |= savedBits[lvl];
}

void bbia_bits_shift_right (bbia * self, int value) {

	if (self == NULL) {
		throw("null pointer in bbia_bits_shift_right()");
		return;
	}

	int savedBits [BBIA_LEVEL_TOP-1];

	int bitMask[2] = {0,0};
	for (int curBit = 1; curBit <= value; curBit++)
		bitMask[0] |= stuaa_bitflag(curBit);

	// 1. Shift with saved bits
	// we shift all levels from zero to top
	// but save the part which is lost
	// we saved bits in position 1...value
	// but need set bits in position INTEGER_SIZE-value...INTEGER_SIZE
	// top level not count in saving

	// 2. Set saved bits
	// for all levels bigger then 0
	// we set saved bits

	for (int lvl = 0; lvl < BBIA_LEVEL_TOP; stuaa_shiftr (self->at+lvl,value), lvl++) {
		bitMask[1] = self->at[lvl] & bitMask[0];
		bitMask[1] <<= BBIA_INTEGER_SIZE-value;
		savedBits[lvl] = bitMask[1];
	}
	stuaa_shiftr (self->at+BBIA_LEVEL_TOP,value);

	for (int lvl = 0; lvl < BBIA_LEVEL_TOP; lvl++)
		self->at[lvl+1] |= savedBits[lvl];
}

// @BBIA_SUM_INT

void bbia_sum_int_levelOut (bbia * self, int integer, int fromLvl, int prevLvl) {

	if (prevLvl != 0) {
		if (stuaa_outofbounders_max(self->at[prevLvl-1],1) == 1) {
			bbia_sum_int_levelOut (self, integer, fromLvl, prevLvl-1);
			return;
		}
		else
			self->at[prevLvl-1]++;
	}

	for (int curLvl = prevLvl; curLvl < fromLvl; curLvl++)
		self->at[curLvl] = BBIA_LEVEL_IS_EMPTY;

	// x = x + y
	// the number needed to overflow is z = FULL - y + 1
	// the value to set is x = EMPTY + x - z
	self->at[fromLvl] = BBIA_LEVEL_IS_EMPTY + self->at[fromLvl] - (BBIA_LEVEL_IS_FULL - integer + 1);
}

void bbia_dif_int_levelOut (bbia * self, int integer, int fromLvl, int prevLvl) {

	if (prevLvl != 0) {
		if (stuaa_outofbounders_min(self->at[prevLvl-1],1) == 1) {
			bbia_dif_int_levelOut (self, integer, fromLvl, prevLvl-1);
			return;
		}
		else
			self->at[prevLvl-1]--;
	}

	if (prevLvl == 1) bbia_sign_change (self);

	for (int curLvl = prevLvl; curLvl < fromLvl; curLvl++)
		self->at[curLvl] = BBIA_LEVEL_IS_EMPTY;

	// x = x - y
	// the number needed to overflow is z = EMPTY + y - 1
	// the value to set x = FULL - x + z
	self->at[fromLvl] = BBIA_LEVEL_IS_FULL - self->at[fromLvl] + (BBIA_LEVEL_IS_EMPTY + integer - 1);

	// @TODO CHECK
	for (int curLvl = prevLvl+1; curLvl <= BBIA_LEVEL_TOP; curLvl++)
		self->at[curLvl] = BBIA_LEVEL_IS_FULL - self->at[curLvl] + 1;
}


void bbia_sum_int_level (bbia * self, int integer, int level) {

	if (stuaa_outofbounders_max (self->at[level], integer) == 0)
		self->at[level] += integer;
	else
		bbia_sum_int_levelOut (self, integer, level, level);
}

void bbia_dif_int_level (bbia * self, int integer, int level) {

	if (stuaa_outofbounders_min (self->at[level], integer) == 0) {
		printf("%d - %d\n", self->at[level], integer);
		self->at[level] -= integer;
	}
	else
		bbia_dif_int_levelOut (self, integer, level, level);
}

void bbia_sum_int (bbia * self, int integer) {

	if (self == NULL) {
		throw ("null pointer in bbia_sum_int");
		return;
	}

	if (self->sign == 0)
		bbia_sum_int_level (self, integer, BBIA_LEVEL_TOP);
	else if (self->sign == 1)
		bbia_dif_int_level (self, integer, BBIA_LEVEL_TOP);
}

void bbia_dif_int (bbia * self, int integer) {

	if (self == NULL) {
		throw ("null pointer in bbia_dif_int");
		return;
	}

	if (self->sign == 0)
		bbia_dif_int_level (self, integer, BBIA_LEVEL_TOP);
	else if (self->sign == 1)
		bbia_sum_int_level (self, integer, BBIA_LEVEL_TOP);
}

// @MULT

static inline bbia * bbia_multiplicationByBitAnd_operation (bbia * self, bbia * temp) {

	// Integer x BBIA
	// 101 x 010
	// 101 x 01(0) == 101 & 000 = 000
	// 101 x 0(1)0 == 1010 & 1111 = 1010
	// 101 x (0)10 == 10100 & 00000 = 00000
	// 000 + 1010 + 00000 = 1010 (101 x 010)
	// See calculation folder for more information

	bbia * result = bbia_new();
	int curBitVal = 0;

	for (
		int curBit = 1, lvl = BBIA_LEVEL_TOP,
		lvlBit = curBit % BBIA_INTEGER_SIZE;

		curBit <= BBIA_BITS_COUNT;

		curBit++, lvlBit = curBit % BBIA_INTEGER_SIZE
	) {
		if (lvlBit != 0)
			curBitVal = (self->at[lvl] & stuaa_bitflag (lvlBit)) ? 1 : 0;
		else
			curBitVal = (self->at[lvl--] & stuaa_bitflag (BBIA_INTEGER_SIZE)) ? 1 : 0;

		if (curBitVal == 1)
			bbia_sum_bbia (result, temp);

		bbia_bits_shift_left(temp, 1);
	}

	return result;
}

bbia * bbia_mult_int_new (bbia * self, int integer) {

	if (self == NULL) {
		throw ("null pointer in bbia_mult_int_new");
		return NULL;
	}

	bbia * integerValue = bbia_new();
	integerValue->at[BBIA_LEVEL_TOP] = integer;

	bbia * result = bbia_multiplicationByBitAnd_operation (self, integerValue);
	bbia_free (integerValue);
	return result;
}

void bbia_mult_int (bbia * self, int integer) {

	bbia * res = bbia_mult_int_new (self, integer);

	bbia_copy_bbia (self, res);

	bbia_free (res);
}

// @BBIA_MULT_BBIA

bbia * bbia_mult_bbia_new (bbia * first, bbia * second) {

	if (first == NULL || second == NULL) {
		throw ("null pointer in bbia_mult_bbia_new");
		return NULL;
	} else if (first == second)
		return bbia_pow_new (first, 2);

	bbia * temp = bbia_copy_new (second);
	bbia * result = bbia_multiplicationByBitAnd_operation (first, temp);
	bbia_free (temp);

	if ((first->sign != second->sign) || (first->sign == 1 && second->sign == 1)) bbia_sign_change (result);
	return result;
}

void bbia_mult_bbia (bbia * to, bbia * second) {

	if (to == second) {
		bbia_pow (to, 2);
		return;
	}

	int toChangeSign = ((to->sign != second->sign) || (to->sign == 1 && second->sign == 1)) ? 1 : 0;
	bbia * res = bbia_multiplicationByBitAnd_operation (second, to);

	bbia_copy_bbia (to, res);
	bbia_free (res);

	if (toChangeSign == 1) bbia_sign_change (to);
}

// @BBIA_DIV

void bbia_divisionBy_operation (bbia * self, bbia * division, DIVISION_MOD flag) {

	bbia * mod = bbia_copy_new(self);

	// set result to zero and work with it like with current multiplication of division
	bbia_set_value (self, 0);
	bbia * currentMultiplierOfDivision = self; // sugar
	currentMultiplierOfDivision->at[BBIA_LEVEL_TOP] = 2;

	// create temporary to store what must be subtrahended from mod
	bbia * currentDifference = bbia_copy_new (division);

	for (; bbia_compare_bbia (mod, currentDifference) != 1;
	       bbia_sum_int (currentMultiplierOfDivision, 1) /*analog for i++)*/)
	{
		bbia_copy_bbia (currentDifference, division);
		bbia_mult_bbia (currentDifference, currentMultiplierOfDivision);
	}

	bbia_dif_int (currentMultiplierOfDivision, 2); // analog for i--
	// division result
	if (flag == DIVISION) {
		bbia_free(currentDifference);
		bbia_free (mod);
		return;
	}

	// mod result
	else if (flag == MOD) {
		bbia_copy_bbia (currentDifference, division);
		bbia_mult_bbia (currentDifference, currentMultiplierOfDivision);
	 	bbia_print_levelValue_dec (currentDifference);
		bbia_dif_bbia (mod, currentDifference);
		bbia_free(currentDifference);

		bbia_free (self);
		self = mod;
	}
}

void bbia_div_bbia (bbia * divided, bbia * division) {

	bbia_divisionBy_operation (divided, division, DIVISION);
}

void bbia_mod_bbia (bbia * divided, bbia * division) {

	bbia_divisionBy_operation (divided, division, MOD);
}

bbia * bbia_div_bbia_new (bbia * divided, bbia * division) {

	bbia * res = bbia_copy_new (divided);
	bbia_divisionBy_operation (res, division, DIVISION);
	return res;
}

bbia * bbia_mod_bbia_new (bbia * divided, bbia * division) {

	bbia * res = bbia_copy_new (divided);
	bbia_divisionBy_operation (res, division, MOD);
	return res;
}

// @BBIA_DIV_INT

void bbia_div_int (bbia * self, int integer) {

	bbia * division = bbia_new();
	division->at[BBIA_LEVEL_TOP] = integer;

	bbia_divisionBy_operation (self, division, DIVISION);
	bbia_free (division);
}

bbia * bbia_div_int_new (bbia * self, int integer) {

	bbia * res = bbia_copy_new (self);
	bbia_div_int (res, integer);
	return res;
}

void bbia_mod_int (bbia * self, int integer) {

	bbia * division = bbia_new();
	division->at[BBIA_LEVEL_TOP] = integer;

	bbia_divisionBy_operation (self, division, MOD);
	bbia_free (division);
}

bbia * bbia_mod_int_new (bbia * self, int integer) {

	bbia * res = bbia_copy_new (self);
	bbia_mod_int (res, integer);
	return res;
}

// @BITFLAG

void bbia_bits_flag_set (bbia * self, int num) {

	if (self == NULL) {
		throw("null pointer in bbia_bits_flag_set()");
		return;
	}

	int lvl = BBIA_LEVEL_TOP - num / BBIA_INTEGER_SIZE;
	num %= BBIA_INTEGER_SIZE;

	if (num != 0)
		self->at[lvl] |= stuaa_bitflag (num);
	else
		self->at[lvl+1] |= stuaa_bitflag (BBIA_INTEGER_SIZE);
}

void bbia_bits_flag_unset (bbia * self, int num) {

	if (self == NULL) {
		throw("null pointer in bbia_bits_flag_unset()");
		return;
	}

	int lvl = BBIA_LEVEL_TOP - num / BBIA_INTEGER_SIZE;
	num %= BBIA_INTEGER_SIZE;

	if (num != 0)
		self->at[lvl] &= ~stuaa_bitflag (num);
	else
		self->at[lvl+1] &= ~stuaa_bitflag (BBIA_INTEGER_SIZE);

}

void bbia_bits_flag_set_mult (bbia * self, int * numArray) {

	if (self == NULL || numArray == NULL) {
		throw("null pointer in bbia_bits_flag_set_mult()");
		return;
	}

	while (numArray != NULL)
		bbia_bits_flag_set (self, *numArray++);
}

void bbia_bits_flag_unset_mult (bbia * self, int * numArray) {

	if (self == NULL || numArray == NULL) {
		throw("null pointer in bbia_bits_flag_unset_mult()");
		return;
	}

	while (numArray != NULL)
		bbia_bits_flag_unset (self, *numArray++);
}


bbia * bbia_bits_flag (int num) {

	bbia * self = bbia_new ();

	int lvl = BBIA_LEVEL_TOP - num / BBIA_INTEGER_SIZE;
	num %= BBIA_INTEGER_SIZE;

	if (num != 0)
		self->at[lvl] |= stuaa_bitflag (num);
	else
		self->at[lvl+1] |= stuaa_bitflag (BBIA_INTEGER_SIZE);

	return self;
}

// AND OR

void bbia_bits_or (bbia * first, bbia * second) {

	if (first == NULL || second == NULL) {
		throw ("null pointer in bbia_or_int");
		return;
	}

	for (int curLvl = BBIA_LEVEL_TOP; curLvl >= 0; curLvl--)
		first->at[curLvl] |= second->at[curLvl];
}

bbia * bbia_bits_or_new (bbia * first, bbia * second) {

	if (first == NULL || second == NULL) {
		throw ("null pointer in bbia_or_int_new");
		return NULL;
	}

	bbia * self = bbia_new();

	for (int curLvl = BBIA_LEVEL_TOP; curLvl >= 0; curLvl--)
		self->at[curLvl] = first->at[curLvl] | second->at[curLvl];

	return self;
}

void bbia_bits_and (bbia * first, bbia * second) {

	if (first == NULL || second == NULL) {
		throw ("null pointer in bbia_and_int");
		return;
	}

	for (int curLvl = BBIA_LEVEL_TOP; curLvl >= 0; curLvl--)
		first->at[curLvl] &= second->at[curLvl];
}

bbia * bbia_bits_and_new (bbia * first, bbia * second) {

	if (first == NULL || second == NULL) {
		throw ("null pointer in bbia_and_int_new");
		return NULL;
	}

	bbia * self = bbia_new();

	for (int curLvl = BBIA_LEVEL_TOP; curLvl >= 0; curLvl--)
		self->at[curLvl] = first->at[curLvl] & second->at[curLvl];

	return self;
}

// bbia full | bbia is empty

bbia * bbia_bits_isFull () {

	bbia * self = bbia_new();
	for (int curLvl = BBIA_LEVEL_TOP; curLvl >= 0; curLvl--)
		self->at[curLvl] = BBIA_LEVEL_IS_FULL;

	return self;
}

bbia * bbia_bits_isEmpty () {

	bbia * self = bbia_new();
	for (int curLvl = BBIA_LEVEL_TOP; curLvl >= 0; curLvl--)
		self->at[curLvl] = BBIA_LEVEL_IS_EMPTY;

	return self;
}

// bbia full | bbia is empty to bit

bbia * bbia_bits_tillBit_isFull (int num) {

	bbia * self = bbia_new();

	int lvl = BBIA_LEVEL_TOP - num / BBIA_INTEGER_SIZE;
	num %= BBIA_INTEGER_SIZE;
	if (num == 0) lvl++;

	for (int curLvl = BBIA_LEVEL_TOP; curLvl > lvl; curLvl--)
		self->at[curLvl] = BBIA_LEVEL_IS_FULL;

	for (int curBit = 1; curBit <= num; curBit++)
		self->at[lvl] |= stuaa_bitflag (curBit);

	return self;
}

bbia * bbia_bits_tillBit_isEmpty (int num) {

	bbia * self = bbia_new();

	int lvl = BBIA_LEVEL_TOP - num / BBIA_INTEGER_SIZE;
	num %= BBIA_INTEGER_SIZE;
	if (num == 0) lvl++;

	for (int curLvl = BBIA_LEVEL_TOP; curLvl > lvl; curLvl--)
		self->at[curLvl] = BBIA_LEVEL_IS_EMPTY;

	for (int curBit = 1; curBit <= num; curBit++)
		self->at[lvl] &= ~(stuaa_bitflag (curBit));

	return self;
}

// @BBIA_SUM_BBIA

void bbia_sum_bbia (bbia * first, bbia * second) {

	if (first == NULL || second == NULL) {
		throw ("null pointer in bbia_sum_bbia");
		return;
	}

	if (first->sign == second->sign)
		for (int curLvl = 0; curLvl <= BBIA_LEVEL_TOP; curLvl++)
			bbia_sum_int_level (first, second->at[curLvl], curLvl);
	else
		for (int curLvl = 0; curLvl <= BBIA_LEVEL_TOP; curLvl++)
			bbia_dif_int_level (first, second->at[curLvl], curLvl);
}

void bbia_sum_bbia_to (bbia * to, bbia * first, bbia * second) {

	if (to == NULL || first == NULL || second == NULL) {
		throw ("null pointer in bbia_sum_bbia");
		return;
	}

	bbia_copy_bbia (to, first);


	if (first->sign == second->sign)
		for (int curLvl = 0; curLvl <= BBIA_LEVEL_TOP; curLvl++)
			bbia_sum_int_level (to, second->at[curLvl], curLvl);
	else
		for (int curLvl = 0; curLvl <= BBIA_LEVEL_TOP; curLvl++)
			bbia_dif_int_level (to, second->at[curLvl], curLvl);
}

bbia * bbia_sum_bbia_new (bbia * first, bbia * second) {

	bbia * to = bbia_new();
	bbia_sum_bbia_to (to, first, second);
	return to;
}

void bbia_dif_bbia (bbia * first, bbia * second) {

	if (first == NULL || second == NULL) {
		throw ("null pointer in bbia_sum_bbia");
		return;
	}

	if (first->sign == second->sign)
		for (int curLvl = 0; curLvl <= BBIA_LEVEL_TOP; curLvl++)
			bbia_dif_int_level (first, second->at[curLvl], curLvl);
	else
		for (int curLvl = 0; curLvl <= BBIA_LEVEL_TOP; curLvl++)
			bbia_sum_int_level (first, second->at[curLvl], curLvl);
}

void bbia_dif_bbia_to (bbia * to, bbia * first, bbia * second) {

	if (to == NULL || first == NULL || second == NULL) {
		throw ("null pointer in bbia_sum_bbia");
		return;
	}

	bbia_copy_bbia (to, first);


	if (first->sign == second->sign)
		for (int curLvl = 0; curLvl <= BBIA_LEVEL_TOP; curLvl++)
			bbia_dif_int_level (to, second->at[curLvl], curLvl);
	else
		for (int curLvl = 0; curLvl <= BBIA_LEVEL_TOP; curLvl++)
			bbia_sum_int_level (to, second->at[curLvl], curLvl);
}

bbia * bbia_dif_bbia_new (bbia * first, bbia * second) {

	bbia * to = bbia_new();
	bbia_dif_bbia_to (to, first, second);
	return to;
}

// @BBIA_POW
void bbia_pow (bbia * self, int power) {
	if (self == NULL) {
		throw("null pointer in bbia_pow()");
		return;
	}
	bbia * temp = bbia_copy_new (self);
	bbia * saved = bbia_copy_new (self);

	while (power-- != 1) {
		bbia_mult_bbia (self, temp);
		bbia_copy_bbia (temp, saved);
	}
	bbia_free (temp);
	bbia_free (saved);
}

bbia * bbia_pow_new (bbia * self, int power) {

	bbia * res = bbia_copy_new (self);
	bbia_pow (res, power);
	return res;
}

// @SET

void bbia_set_value_fromLevel (bbia * self, int level, int value) {

	if (self == NULL) {
		throw("null pointer in bbia_set_value_fromLevel()");
		return;
	}

	for (int curLvl = level; curLvl >= 0; curLvl--)
		self->at[curLvl] = value;
}

void bbia_set_value_toLevel (bbia * self, int level, int value) {

	if (self == NULL) {
		throw("null pointer in bbia_set_value_fromLevel()");
		return;
	}

	for (int curLvl = level; curLvl <= BBIA_LEVEL_TOP; curLvl++)
		self->at[curLvl] = value;
}

void bbia_set_value (bbia * self, int value) {

	bbia_set_value_fromLevel (self, BBIA_LEVEL_TOP, value);
}

int bbia_at_get (bbia * self, int index) {

	if (self == NULL) {
		throw("null pointer in bbia_at_get()");
		return -1;
	}

	return self->at[index];
}

void bbia_at_set (bbia * self, int index, int value) {

	if (self == NULL) {
		throw("null pointer in bbia_at_set()");
		return;
	}

	self->at[index] = value;
}

// @PRINT

void bbia_print_levelValue (bbia * self) {

	if (self == NULL) {
		throw("null pointer in bbia_print_levelValue()");
		return;
	}

	char * tempMemory = NULL;

	for (int curLvl = 0; curLvl <= BBIA_LEVEL_TOP; curLvl++) {

		tempMemory = stuaa_toBase (self->at[curLvl], 2);
		printf ("%s_", tempMemory);

		if (tempMemory != NULL) free (tempMemory);
	}

	puts("");
}

void bbia_print_levelValue_dec (bbia * self) {

	if (self == NULL) {
		throw("null pointer in bbia_print_levelValue()");
		return;
	}

	for (int curLvl = 0; curLvl <= BBIA_LEVEL_TOP; curLvl++)
		printf ("%u_", self->at[curLvl]);

	puts("");
}

char * bbia_toBase (bbia * self, int base) {

	if (self == NULL) {
		throw("null pointer in bbia_toBase()");
		return NULL;
	}

	if ( !(base < 65 && base > 1) ) {
		throw ("The base must be from 2 to 64");
		return NULL;
	}

	int powerOfTwo = stuaa_isPowerOfTwo (base);
	if (base == 2) {

		char * result = calloc (sizeof(char), BBIA_BITS_COUNT + 1);
		if (result == NULL) abort();

		bbia * curBitFlag = NULL;
		bbia * curBitAndFlag = NULL;
		for (int currentBit = 1; currentBit <= BBIA_BITS_COUNT; currentBit++) {

			curBitFlag = bbia_bits_flag (currentBit);
			curBitAndFlag = bbia_bits_and_new(self, curBitFlag);

			result[BBIA_BITS_COUNT-currentBit] =
			bbia_is_zero (curBitAndFlag) ? '0' : '1';

			bbia_free (curBitAndFlag);
			bbia_free (curBitFlag);
		}

		return result;
	}

	else if (powerOfTwo != -2) {

		int start = log_formMax();

		char * result = calloc (sizeof(char), start + 1);
		if (result == NULL) abort();

		int tempValue = 0;
		bbia * curBitFlag = NULL;
		bbia * curBitAndFlag = NULL;
		for (int curBit = 0; curBit < BBIA_BITS_COUNT; curBit += powerOfTwo) {
			for (int curBitInTwo = 1; curBitInTwo <= powerOfTwo; curBitInTwo++) {
				curBitFlag = bbia_bits_flag (curBit + curBitInTwo);
				curBitAndFlag = bbia_bits_and_new(self, curBitFlag);

				tempValue |=
				bbia_is_zero (curBitAndFlag) ? '0' : stuaa_bitflag (curBitInTwo);

				bbia_free (curBitAndFlag);
				bbia_free (curBitFlag);
			}

			result[start--] = numerics[tempValue];
			tempValue = 0;
		}

		// shift for needed position
		// @TODO

		return result;

	}

	return NULL;
}

bbia * bbia_fromBase (char * self, int base) {

	if (self == NULL) {
		throw("null pointer in bbia_fromBase()");
		return NULL;
	}

	if ( !(base < 65 && base > 1) ) {
		throw ("The base must be from 2 to 64");
		return NULL;
	}

	int powerOfTwo = stuaa_isPowerOfTwo (base);
	if (base == 2) {

	}

	else if (powerOfTwo != -2) {


	}

	return NULL;
}

// @SIGN

int bbia_sign_is (bbia * self) {

	if (self == NULL) {
		throw("null pointer in bbia_sign_is()");
		return -1;
	}

	return self->sign;
}

void bbia_sign_change (bbia * self) {

	if (self == NULL) {
		throw("null pointer in bbia_sign_change()");
		return;
	}

	self->sign = (self->sign) ? 0 : 1;
}

void bbia_sign_set (bbia * self, int sign) {

	if (self == NULL) {
		throw("null pointer in bbia_sign_set()");
		return;
	}

	self->sign = sign;
}

// @COPY

void bbia_copy_bbia (bbia * to, bbia * from) {

	if (from == NULL || to == NULL) {
		throw("null pointer in bbia_copy_bbia()");
		return;
	}

	for (int curLvl = 0; curLvl <= BBIA_LEVEL_TOP; curLvl++)
		to->at[curLvl] = from->at[curLvl];

	to->sign = from->sign;
}

bbia * bbia_copy_new (bbia * from) {

	if (from == NULL) {
		throw("null pointer in bbia_copy_new()");
		return NULL;
	}

	bbia * to = bbia_new();
	bbia_copy_bbia (to, from);
	return to;
}

// @CHECK

int bbia_is_integer (bbia * self, int integer) {

	if (self == NULL) {
		throw("null pointer in bbia_is_integer()");
		return 0;
	}

	for (int curLvl = 0; curLvl < BBIA_LEVEL_TOP; curLvl++)
		if (self->at[curLvl] != BBIA_LEVEL_IS_EMPTY)
			return 0;

	return (self->at[BBIA_LEVEL_TOP] == integer) ? 1 : 0;
}

int bbia_is_zero (bbia * self) {

	return bbia_is_integer (self, 0);
}

int bbia_is_one (bbia * self) {

	return bbia_is_integer (self, 1);
}

// @COMPARE

static inline int bbia_compare_bbia_op (bbia * a, bbia * b) {

	int curCompare = 0;

	for (int curLvl = 0; curLvl <= BBIA_LEVEL_TOP; curLvl++) {
		curCompare = stuaa_compare(a->at[curLvl], b->at[curLvl]);

		if (curCompare != 0)
			return curCompare;
	}
	return 0;
}

int bbia_compare_bbia (bbia * a, bbia * b) {

	int res = bbia_compare_bbia_op (a,b);
	if (a->sign == 0 && b->sign == 0) return res;

	if (a->sign == b->sign) return (res == -1) ? 1 : -1;

	if (a->sign > b->sign) return (res == 1) ? 1 : -1;

	return 0;
}

static inline int bbia_compare_int_op (bbia * self, int toCompare) {

	int curCompare = stuaa_compare(self->at[BBIA_LEVEL_TOP], toCompare);
	if (curCompare == 1) return 1;

	else {
		for (int curLvl = BBIA_LEVEL_TOP; curLvl >= 0; curLvl--)
			if (self->at[curLvl] != 0) return 1;
	}
	return curCompare;
}

int bbia_compare_int (bbia * self, int toCompare, int isSigned) {

	int res = bbia_compare_int_op (self, toCompare);
	if (isSigned == 0 && self->sign == 0) return res;

	if (self->sign == isSigned) return (res == -1) ? 1 : -1;

	if (self->sign > isSigned) return (res == 1) ? 1 : -1;

	return 0;
}

// @CONSTRUCTOR

bbia * bbia_new (void) {

	bbia * self = calloc (sizeof(bbia), 1);
	if (self == NULL) abort();

	self->sign = BBIA_UNSIGNED;
	bbia_set_value (self, BBIA_LEVEL_IS_EMPTY);

	return self;
}

void bbia_free (bbia * self) {

	if (self == NULL) {
		throw("null pointer in bbia_free()");
		return;
	}

	free(self);
}
