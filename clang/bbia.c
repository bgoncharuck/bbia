#include "bbia.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define throw(MSG) fprintf(stderr, "%s\n",MSG)

struct __bbia {
	unsigned at[BBIA_LEVEL_COUNT];
	unsigned sign;
};

// @STUAA

static const char * numerics =
"0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

static inline double log_base(double base, double num) {
	return log2(num) / log2(base);
}

static inline int findDigitInNumerics (const char * numicsStr, char digit) {
	for ( unsigned curNumicsInd = 0; curNumicsInd < BBIA_INTEGER_SIZE;
		curNumicsInd++)

	if (numicsStr [curNumicsInd] == digit)
		return curNumicsInd;

	return -1;
}

static inline unsigned bitflag (unsigned num) {

	if ( !(num >= 0 && num <= BBIA_INTEGER_SIZE) )
		return 0;

	if (BBIA_INTEGER_SIZE < 32 || num < 32) {

		const unsigned bitDigit[] = { 0,
			0x1, 0x2, 0x4, 0x8,
			0x10, 0x20, 0x40, 0x80,
			0x100, 0x200, 0x400, 0x800,
			0x1000, 0x2000, 0x4000, 0x8000,
			0x10000, 0x20000, 0x40000, 0x80000,
			0x100000, 0x200000, 0x400000, 0x800000,
			0x1000000, 0x2000000, 0x4000000, 0x8000000,
			0x10000000, 0x20000000, 0x40000000, 0x80000000
		};

		return bitDigit[num];
	}

	unsigned bitDigit = 0x40000000;

	while (num-- != 31)
		bitDigit <<= 1;

	return bitDigit;
}

static char * toBase (unsigned integer, unsigned base) {

	if ( !(base < 65 && base > 1) ) {
		throw ("The base must be from 2 to 64");
		return NULL;
	}

	char * result = malloc (sizeof(char) * BBIA_INTEGER_SIZE + 1);
	if (result == NULL) abort();

	if (base == 2) {
		for (unsigned currentBit = 1; currentBit <= BBIA_INTEGER_SIZE;

			result[BBIA_INTEGER_SIZE-currentBit] =
			(integer & bitflag(currentBit)) ? '1' : '0',
			currentBit++
		);

		return result;
	}

	for (unsigned start = ceil ( log_base (base, integer) ) - 1; integer != 0;

		result[start] = numerics [ integer % base ],
		integer /= base,
		start--
	);

	return result;
}

static unsigned fromBase (char * integer, unsigned base) {

	if (integer == NULL) {
		throw ("null pointer in fromBase()");
		return -1;
	}

	unsigned result = 0;

	if (base == 2) {
		for (unsigned currentBit = 1; currentBit <= BBIA_INTEGER_SIZE;

			result |= (integer[BBIA_INTEGER_SIZE-currentBit] == '1')
			? bitflag(currentBit) : 0,
			currentBit++
		);

		return result;
	}

	for ( unsigned curDigit = 0; curDigit < BBIA_INTEGER_SIZE;

		result *= base,
		result += findDigitInNumerics (numerics, integer[curDigit]),
		curDigit++
	);

	return result;
}

// @BBIA

void bbia_bitshift_left (bbia * self, int value) {

	if (self == NULL) {
		throw("null pointer in bbia_bitshift_left()");
		return;
	}

	unsigned savedBits [BBIA_LEVEL_TOP-1];

	unsigned bitMask[2] = {0,0};
	for (unsigned curBit = BBIA_INTEGER_SIZE-value; curBit < BBIA_INTEGER_SIZE; curBit++)
		bitMask[0] |= 0x1 << curBit;

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
		bitMask[1] >>= BBIA_INTEGER_SIZE-value;
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

	unsigned savedBits [BBIA_LEVEL_TOP-1];

	unsigned bitMask[2] = {0,0};
	for (unsigned curBit = 0; curBit < value; curBit++)
		bitMask[0] |= 0x1 << curBit;

	// 1. Shift with saved bits
	// we shift all levels from zero to top
	// but save the part which is lost
	// we saved bits in position 1...value
	// but need set bits in position INTEGER_SIZE-value...INTEGER_SIZE
	// top level not count in saving

	// 2. Set saved bits
	// for all levels bigger then 0
	// we set saved bits

	for (int lvl = 0; lvl <= BBIA_LEVEL_TOP; self->at[lvl] >>= value, lvl++)
	if (lvl < BBIA_LEVEL_TOP) {
		bitMask[1] = self->at[lvl] & bitMask[0];
		bitMask[1] <<= BBIA_INTEGER_SIZE-value;
		savedBits[lvl] = bitMask[1];
	}

	for (int lvl = 0; lvl < BBIA_LEVEL_TOP; lvl++)
		self->at[lvl+1] |= savedBits[lvl];
}

void bbia_bitflag_set (bbia * self, unsigned num) {

	if (self == NULL) {
		throw("null pointer in bbia_bitflag_set()");
		return;
	}

	unsigned lvl = BBIA_LEVEL_TOP - num / BBIA_INTEGER_SIZE;
	num %= BBIA_INTEGER_SIZE;

	if (num != 0)
		self->at[lvl] |= bitflag (num);
	else
		self->at[lvl+1] |= BBIA_BIT_LAST;
}

void bbia_bitflag_unset (bbia * self, unsigned num) {

	if (self == NULL) {
		throw("null pointer in bbia_bitflag_unset()");
		return;
	}

	unsigned lvl = BBIA_LEVEL_TOP - num / BBIA_INTEGER_SIZE;
	num %= BBIA_INTEGER_SIZE;

	if (num != 0)
		self->at[lvl] &= ~bitflag (num);
	else
		self->at[lvl+1] &= ~BBIA_BIT_LAST;

}

void bbia_bitflag_set_mult (bbia * self, unsigned * numArray) {

	if (self == NULL || !numArray) {
		throw("null pointer in bbia_bitflag_set_mult()");
		return;
	}

	while (numArray != NULL)
		bbia_bitflag_set (self, *numArray++);
}

void bbia_bitflag_unset_mult (bbia * self, unsigned * numArray) {

	if (self == NULL || !numArray) {
		throw("null pointer in bbia_bitflag_unset_mult()");
		return;
	}

	while (numArray != NULL)
		bbia_bitflag_unset (self, *numArray++);
}

bbia * bbia_bitflag (unsigned num) {

	bbia * self = bbia_new ();

	unsigned lvl = BBIA_LEVEL_TOP - num / BBIA_INTEGER_SIZE;
	num %= BBIA_INTEGER_SIZE;

	if (num != 0)
		self->at[lvl] |= bitflag (num);
	else
		self->at[lvl+1] |= BBIA_BIT_LAST;

	return self;
}

void bbia_set_value_fromLevel (bbia * self, unsigned level, int value) {

	if (self == NULL) {
		throw("null pointer in bbia_set_value_fromLevel()");
		return;
	}

	for (int curLvl = level; curLvl >= 0; curLvl--)
		self->at[curLvl] = value;
}

void bbia_set_value_toLevel (bbia * self, unsigned level, int value) {

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

int bbia_at_get (bbia * self, unsigned index) {

	if (self == NULL) {
		throw("null pointer in bbia_at_get()");
		return -1;
	}

	return self->at[index];
}

void bbia_at_set (bbia * self, unsigned index, int value) {

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

		tempMemory = toBase (self->at[curLvl], 2);
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
