#include "stuaa.h"
#include "bbia.h"

#include <stdio.h>
#include <stdlib.h>
// #include <string.h>

#define throw(MSG) fprintf(stderr, "%s\n",MSG)

struct __bbia {

	// some languages don`t have unsigned int
	signed int at[BBIA_LEVEL_COUNT];
	signed int sign;
};

// @BITSHIFT

void bbia_bitshift_left (bbia * self, int value) {

	if (self == NULL) {
		throw("null pointer in bbia_bitshift_left()");
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

	for (int lvl = BBIA_LEVEL_TOP; lvl >= 0; self->at[lvl] <<= value, lvl--)
	if (lvl > 0) {
		bitMask[1] = self->at[lvl] & bitMask[0];
		stuaa_shiftr (bitMask+1, BBIA_INTEGER_SIZE-value);
		savedBits[lvl-1] = bitMask[1];
	}

	for (int lvl = 0; lvl < BBIA_LEVEL_TOP; lvl++)
		self->at[lvl] |= savedBits[lvl];
}

void bbia_bitshift_right (bbia * self, int value) {

	if (self == NULL) {
		throw("null pointer in bbia_bitshift_right()");
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

	for (int lvl = 0; lvl <= BBIA_LEVEL_TOP; stuaa_shiftr (self->at+lvl,value), lvl++)
	if (lvl < BBIA_LEVEL_TOP) {
		bitMask[1] = self->at[lvl] & bitMask[0];
		bitMask[1] <<= BBIA_INTEGER_SIZE-value;
		savedBits[lvl] = bitMask[1];
	}

	for (int lvl = 0; lvl < BBIA_LEVEL_TOP; lvl++)
		self->at[lvl+1] |= savedBits[lvl];
}

// @BITFLAG

void bbia_bitflag_set (bbia * self, int num) {

	if (self == NULL) {
		throw("null pointer in bbia_bitflag_set()");
		return;
	}

	int lvl = BBIA_LEVEL_TOP - num / BBIA_INTEGER_SIZE;
	num %= BBIA_INTEGER_SIZE;

	if (num != 0)
		self->at[lvl] |= stuaa_bitflag (num);
	else
		self->at[lvl+1] |= stuaa_bitflag (BBIA_INTEGER_SIZE);
}

void bbia_bitflag_unset (bbia * self, int num) {

	if (self == NULL) {
		throw("null pointer in bbia_bitflag_unset()");
		return;
	}

	int lvl = BBIA_LEVEL_TOP - num / BBIA_INTEGER_SIZE;
	num %= BBIA_INTEGER_SIZE;

	if (num != 0)
		self->at[lvl] &= ~stuaa_bitflag (num);
	else
		self->at[lvl+1] &= ~stuaa_bitflag (BBIA_INTEGER_SIZE);

}

void bbia_bitflag_set_mult (bbia * self, int * numArray) {

	if (self == NULL || numArray == NULL) {
		throw("null pointer in bbia_bitflag_set_mult()");
		return;
	}

	while (numArray != NULL)
		bbia_bitflag_set (self, *numArray++);
}

void bbia_bitflag_unset_mult (bbia * self, int * numArray) {

	if (self == NULL || numArray == NULL) {
		throw("null pointer in bbia_bitflag_unset_mult()");
		return;
	}

	while (numArray != NULL)
		bbia_bitflag_unset (self, *numArray++);
}


bbia * bbia_bitflag (int num) {

	bbia * self = bbia_new ();

	int lvl = BBIA_LEVEL_TOP - num / BBIA_INTEGER_SIZE;
	num %= BBIA_INTEGER_SIZE;

	if (num != 0)
		self->at[lvl] |= stuaa_bitflag (num);
	else
		self->at[lvl+1] |= stuaa_bitflag (BBIA_INTEGER_SIZE);
}

// @SUM
static void bbia_add_int_out_level (bbia * self, int integer, int previousLevel, int fromLevel) {

	if (stuaa_outofbounders_max(self->at[previousLevel-1],1) == 0) {
		self->at[previousLevel-1]++;

		for (int i = previousLevel; i < fromLevel; i++)
			self->at[i] = BBIA_LEVEL_IS_EMPTY;

		// 997 + 5; 9-5 = 4; 9-7 = 2; 9-4-2-1 = 2; 100[2]
		integer = BBIA_LEVEL_IS_FULL - integer;
		self->at[fromLevel] = BBIA_LEVEL_IS_FULL - self->at[fromLevel];
		self->at[fromLevel] = BBIA_LEVEL_IS_FULL - integer - self->at[fromLevel] - 1;
	}
	else {
		if (previousLevel != 1)
			bbia_add_int_out_level (self, integer, previousLevel-1, fromLevel);
		else {
			// create out of bounders
			bbia_sign_change (self);

			if (self->at[fromLevel] >= integer) {
				self->at[fromLevel] -= integer;
				self->at[fromLevel] = BBIA_LEVEL_IS_FULL - self->at[fromLevel];
			}
			else {
				integer -= self->at[fromLevel];
				self->at[fromLevel] = BBIA_LEVEL_IS_FULL - integer;
			}

			// @TODO check
			if (fromLevel < TOP_LEVEL)
				for (int i = fromLevel + 1; i <= TOP_LEVEL; i++)
					self->at[i] = BBIA_LEVEL_IS_FULL - self->at[i];
		}
	}
}

static void bbia_add_int_level (bbia * self, int integer, int level) {

	if (stuaa_outofbounders_max (self->at[level], integer) == 0) {
		self->at[level] += integer;
		return;
	}
	else bbia_add_int_out (self, integer, level, level);
}

static void bbia_sub_int_out_level (bbia * self, int integer, int previousLevel, int fromLevel) {

	if (self->at[previousLevel-1] > BBIA_LEVEL_IS_EMPTY) {
		self->at[previousLevel-1]--;

		for (int i = previousLevel; i < fromLevel; i++)
			self->at[i] = BBIA_LEVEL_IS_FULL;

		self->at[fromLevel] += integer;
	}
	else {
		if (previousLevel != 1)
			bbia_sub_int_out_level (self, integer, previousLevel-1, fromLevel);
		else {
			// create out of bounders, but all lower levels are zero
			bbia_sign_change (self);

			// TODO check
			for (int i = previousLevel - 1; i <= TOP_LEVEL;  i++)
				if (self->at[i] != 0 && i != fromLevel)
					self->at[i] = BBIA_LEVEL_IS_FULL - self->at[i];

			if (self->at[fromLevel] <= integer) {
				self->at[fromLevel] += integer;
				self->at[fromLevel] = BBIA_LEVEL_IS_EMPTY + self->at[fromLevel];
			}
			else {
				integer += self->at[fromLevel];
				self->at[fromLevel] = BBIA_LEVEL_IS_EMPTY + integer;
			}
		}
	}
}

static void bbia_sub_int_level (bbia * self, int integer, int level) {

	if (stuaa_outofbounders_min (self->at[level], integer) == 0) {
		self->at[level] -= integer;
		return;
	}
	else bbia_sub_int_out (self, integer, level, level);
}

void bbia_sum_int_level (bbia * self, int integer, int level) {

	if (level < 0 || level > BBIA_LEVEL_TOP) {
		throw ("level out of bounders, be sure you choose correct level");
		return;
	}

	if (integer >= 0 && self->sign == 0 || integer < 0 && self->sign == 1) {
		if (integer < 0) integer = ~integer + 1;
		bbia_add_int_level (self, integer);
	}

	else {
		if (integer < 0) integer = ~integer + 1;
		bbia_sub_int_level (self, integer);
	}
}

void bbia_sum_int_level (bbia * self, int integer) {

	if (self == NULL) {
		throw ("null pointer in bbia_sum_int");
		return;
	}

	bbia_sum_int_level (self, integer, BBIA_LEVEL_TOP);
}

// @MULT



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

// @CONSTRUCTOR

bbia * bbia_new (void) {

	bbia * self = malloc (sizeof(bbia));
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
