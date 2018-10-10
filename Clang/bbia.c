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

	int savedBits [BBIA_LEVEL_COUNT-1][value];
	for (int i = 0; i < BBIA_LEVEL_COUNT-1; i++)
		for (int j = 0; j < value; j++)
			savedBits[i][j] = 0;


	// bitshift with bit overflow saving
	for (int lvl = BBIA_LEVEL_TOP; lvl >= 0; self->at[lvl] <<= value, lvl--)
	for (int currentBit = BBIA_INTEGER_SIZE;
		currentBit > BBIA_INTEGER_SIZE-value; currentBit--)
	if (lvl > 0)
		savedBits[lvl-1][-1 + value - (BBIA_INTEGER_SIZE-currentBit)]
		|= self->at[lvl] & stuaa_bitflag (currentBit);

	// set savedBits in start
	for (int lvl = 0; lvl < BBIA_LEVEL_TOP; lvl++)
	for (int currentBit = 1; currentBit <= value; currentBit++)

	self->at[lvl] |=
	(savedBits[lvl][value-currentBit] & stuaa_bitflag(BBIA_INTEGER_SIZE-currentBit+1)) ?
	stuaa_bitflag (currentBit) : 0;
}

void bbia_bitshift_right (bbia * self, int value) {


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
