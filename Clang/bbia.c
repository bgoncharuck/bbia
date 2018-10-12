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


void bbia_bitshift_left (bbia * self, int value) {

	int savedBits [BBIA_LEVEL_TOP-1];

	int bitMask[2] = {0,0}, signedDebug = 0;
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

		if (stuaa_bitflag(BBIA_INTEGER_SIZE) & bitMask[1]) {
			signedDebug = 1;
			bitMask[1] &= ~stuaa_bitflag(BBIA_INTEGER_SIZE);
		}

		bitMask[1] >>= BBIA_INTEGER_SIZE-value;

		if (signedDebug) {
			signedDebug = 0;
			bitMask[1] |= stuaa_bitflag(value);
		}

		savedBits[lvl-1] = bitMask[1];
	}

	for (int lvl = 0; lvl < BBIA_LEVEL_TOP; lvl++)
		self->at[lvl] |= savedBits[lvl];
}

void bbia_bitshift_right (bbia * self, int value) {

	int savedBits [BBIA_LEVEL_TOP-1];

	int bitMask[2] = {0,0}, signedDebug = 0;
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

	for (int lvl = 0; lvl <= BBIA_LEVEL_TOP; lvl++) {
		if (stuaa_bitflag(BBIA_INTEGER_SIZE) & self->at[lvl]) {
			signedDebug = 1;
			self->at[lvl] &= ~stuaa_bitflag(BBIA_INTEGER_SIZE);
		}

		if (lvl < BBIA_LEVEL_TOP) {
			bitMask[1] = self->at[lvl] & bitMask[0];
			bitMask[1] <<= BBIA_INTEGER_SIZE-value;
			savedBits[lvl] = bitMask[1];
		}

		self->at[lvl] >>= value;
		if (signedDebug) {
			signedDebug = 0;
			self->at[lvl] |= stuaa_bitflag(value);
		}
	}

	for (int lvl = 0; lvl < BBIA_LEVEL_TOP; lvl++)
		self->at[lvl+1] |= savedBits[lvl];
}

void bbia_bitflag_set (bbia * self, int num) {

	int lvl = num / BBIA_INTEGER_SIZE;
	num %= BBIA_INTEGER_SIZE;

	self->at[lvl] |= stuaa_bitflag (num);
}

void bbia_bitflag_unset (bbia * self, int num) {

	int lvl = num / BBIA_INTEGER_SIZE;
	num %= BBIA_INTEGER_SIZE;

	self->at[lvl] &= ~stuaa_bitflag (num);
}

void bbia_bitflag_set_mult (bbia * self, int * numArray) {

	while (numArray != NULL)
		bbia_bitflag_set (self, *numArray++);
}

void bbia_bitflag_unset_mult (bbia * self, int * numArray) {

	while (numArray != NULL)
		bbia_bitflag_unset (self, *numArray++);
}

bbia * bbia_bitflag (int num) {

	bbia * self = bbia_new ();

	int lvl = num / BBIA_INTEGER_SIZE;
	num %= BBIA_INTEGER_SIZE;

	self->at[lvl] |= stuaa_bitflag (num);

	return self;
}

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
