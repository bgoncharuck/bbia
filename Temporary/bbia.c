#include "stuaa.h"
#include "bbia.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define throw(MSG) fprintf(stderr, "%s\n",MSG)

#define nullPointer_funcVoid_1 (p, functionName) \
do { \
	if (p == NULL) { \
		fprintf (stderr, "null pointer in %s\n", functionName); \
		return; \
	} \
} while(0)
#define nullPointer_funcVoid_2 (p1, p2, functionName)do{if(p1 == NULL || p2 == NULL){fprintf(stderr, "null pointer in %s\n", functionName);return;}}while(0)
#define nullPointer_funcPointer_1 (p, functionName)do{if(p == NULL){fprintf(stderr, "null pointer in %s\n", functionName);return NULL;}} while(0)
#define nullPointer_funcPointer_2 (p1, p2, functionName)do{if (p1 == NULL || p2 == NULL){fprintf(stderr, "null pointer in %s\n", functionName);return NULL;}}while(0)
#define nullPointer_funcInt_1 (p, functionName)do{if(p == NULL){fprintf(stderr, "null pointer in %s\n", functionName);return -42;}}while(0)
#define nullPointer_funcInt_2 (p1, p2, functionName)do{if(p1 == NULL || p2 == NULL){fprintf(stderr, "null pointer in %s\n", functionName);return -42;}}while(0)


typedef enum __DIVISION_OR_MOD { DIVISION = 2, MOD = 4 } DIVISION_OR_MOD;

struct __bbia {

	// some languages don`t have unsigned int
	signed int at[BBIA_LEVEL_COUNT];
	signed int sign;
};

// @CONSTRUCTOR

bbia * bbia_new (void) {

	bbia * self = calloc (sizeof(bbia), 1);
	if (self == NULL) abort();

	self->sign = BBIA_UNSIGNED;
	bbia_set_value (self, BBIA_LEVEL_IS_EMPTY);

	return self;
}

void bbia_free (bbia * self) {

	nullPointer_funcVoid_1 (self, "bbia_free");
	free(self);
}

// @BIT_OPERATIONS

void bbia_bits_shift_left (bbia * self, int value) {

	nullPointer_funcVoid_1 (self, "bbia_bits_shift_left");

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

	nullPointer_funcVoid_1 (self, "bbia_bits_shift_right");

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

void bbia_bits_flag_set (bbia * self, int num) ;

void bbia_bits_flag_unset (bbia * self, int num) ;

void bbia_bits_flag_set_mult (bbia * self, int * numArray) ;

void bbia_bits_flag_unset_mult (bbia * self, int * numArray) ;

bbia * bbia_bits_flag (int num) ;

void bbia_bits_or (bbia * first, bbia * second) ;

bbia * bbia_bits_or_new (bbia * first, bbia * second) ;

void bbia_bits_and (bbia * first, bbia * second) ;

bbia * bbia_bits_and_new (bbia * first, bbia * second) ;

bbia * bbia_bits_isFull () ;

bbia * bbia_bits_isEmpty () ;

bbia * bbia_bits_tillBit_isFull (int num) ;

bbia * bbia_bits_tillBit_isEmpty (int num) ;
