#include "stuaa.h"
#include "bbia.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define throw(MSG) fprintf(stderr, "%s\n",MSG)

#define nullPointer_funcVoid_1(p, functionName) do { \
	if (p == NULL) { \
		fprintf (stderr, "null pointer in %s\n", functionName); \
		return; \
	} \
} while(0)
#define nullPointer_funcVoid_2(p1, p2, functionName) do{if(p1 == NULL || p2 == NULL){fprintf(stderr, "null pointer in %s\n", functionName);return;}}while(0)
#define nullPointer_funcPointer_1(p, functionName)do{if(p == NULL){fprintf(stderr, "null pointer in %s\n", functionName);return NULL;}} while(0)
#define nullPointer_funcPointer_2(p1, p2, functionName)do{if (p1 == NULL || p2 == NULL){fprintf(stderr, "null pointer in %s\n", functionName);return NULL;}}while(0)
#define nullPointer_funcInt_1(p, functionName)do{if(p == NULL){fprintf(stderr, "null pointer in %s\n", functionName);return -42;}}while(0)
#define nullPointer_funcInt_2(p1, p2, functionName)do{if(p1 == NULL || p2 == NULL){fprintf(stderr, "null pointer in %s\n", functionName);return -42;}}while(0)


typedef enum __DIVISION_OR_MOD { DIVISION = 2, MOD = 4 } DIVISION_OR_MOD;

struct __bbia {
	// some languages don`t have unsigned int
	signed int at[BBIA_LEVEL_COUNT];
	signed int sign;
	signed int usedLevelPos;
};

// @CONSTRUCTOR

bbia * bbia_new (void) {

	bbia * self = calloc (sizeof(bbia), 1);
	if (self == NULL) abort();

	self->sign = BBIA_UNSIGNED;
	self->usedLevelPos = BBIA_LEVEL_TOP;
	bbia_set_value (self, BBIA_LEVEL_IS_EMPTY);

	return self;
}

bbia * bbia_new_fromSystemInteger (int integer) {
	bbia * self = bbia_new();
	bbia->at[BBIA_LEVEL_TOP] = integer;
	return result;
}

void bbia_free (bbia * self) {
	nullPointer_funcVoid_1 (self, "bbia_free");
	free(self);
}

// @SETERS

void bbia_set_sign_change (bbia * self) {
	nullPointer_funcVoid_1 (self, "bbia_set_sign_change");
	self->sign = (self->sign) ? 0 : 1;
}

void bbia_set_sign (bbia * self, int sign) {
	nullPointer_funcVoid_1 (self, "bbia_set_sign");
	self->sign = sign;
}

void bbia_set_value_fromLevel (bbia * self, int level, int value) {
	nullPointer_funcVoid_1 (self, "bbia_set_value_fromLevel");

	for (int curLvl = level; curLvl >= 0; curLvl--)
		self->at[curLvl] = value;
	self->usedLevelPos = 0; if (value == 0) bbia_check_usedLevelPosition (self);
}

void bbia_set_value_toLevel (bbia * self, int level, int value) {
	nullPointer_funcVoid_1 (self, "bbia_set_value_toLevel");

	for (int curLvl = level; curLvl <= BBIA_LEVEL_TOP; curLvl++)
		self->at[curLvl] = value;
	self->usedLevelPos = level; if (value == 0) bbia_check_usedLevelPosition (self);
}

void bbia_set_value (bbia * self, int value) {
	bbia_set_value_fromLevel (self, BBIA_LEVEL_TOP, value);
}

int bbia_at_get (bbia * self, int index) {
	nullPointer_funcInt_1 (self, "bbia_at_get");
	return self->at[index];
}

void bbia_at_set (bbia * self, int index, int value) {
	nullPointer_funcVoid_1 (self, "bbia_at_set");
	self->at[index] = value;

	if (self->usedLevelPos > index) self->usedLevelPos = index;
}

// @CHECK

void bbia_check_usedLevelPosition (bbia * self) {
	nullPointer_funcVoid_1 (self, "bbia_check_usedLevelPosition");
	for (int curLvl = 0; curLvl <= BBIA_LEVEL_TOP; curLvl++)
		if (self->at[curLvl] != 0) {
			self->usedLevelPos = curLvl;
			return;
		}
}

int bbia_check_sign (bbia * self) {
	nullPointer_funcInt_1 (self, "bbia_check_sign");
	return self->sign;
}

int bbia_check_is_integer (bbia * self, int integer) {

	nullPointer_funcInt_1 (self, "bbia_check_is_integer")
	if (self->usedLevelPos < BBIA_LEVEL_TOP) return 0;

	return (self->at[BBIA_LEVEL_TOP] == integer) ? 1 : 0;
}

int bbia_check_is_zero (bbia * self) {
	return bbia_check_is_integer (self, 0);
}

int bbia_check_is_one (bbia * self) {
	return bbia_check_is_integer (self, 1);
}

int bbia_check_is_systemInteger (bbia * self, int level) {
	nullPointer_funcInt_1 (self, "bbia_check_is_systemInteger");
	return (self->usedLevelPos == BBIA_LEVEL_TOP) ? 1 : 0;
}

// @COMPARE

static inline int bbia_compare_bbia_op (bbia * a, bbia * b) {
	int curCompare = 0;
	int curLvl = (a->usedLevelPos >= b->usedLevelPos) ? a->usedLevelPos : b->usedLevelPos;
	for (; curLvl <= BBIA_LEVEL_TOP; curLvl++) {
		curCompare = stuaa_compare(a->at[curLvl], b->at[curLvl]);
		if (curCompare != 0)
			return curCompare;
	}
	return 0;
}

int bbia_compare_bbia (bbia * a, bbia * b) {

	nullPointer_funcInt_2 (a, b, "bbia_compare_bbia");
	int res = bbia_compare_bbia_op (a,b);

	if (a->sign == 0 && b->sign == 0) return res;
	else if (a->sign == b->sign) return (res == -1) ? 1 : -1;
	else if (a->sign > b->sign) return (res == 1) ? 1 : -1;
	return 0;
}

static inline int bbia_compare_int_op (bbia * self, int toCompare) {
	if (self->usedLevelPos < BBIA_LEVEL_TOP) return 1;
	int curCompare = stuaa_compare(self->at[BBIA_LEVEL_TOP], toCompare);
	return curCompare;
}

int bbia_compare_int (bbia * self, int toCompare, int isSigned) {

	nullPointer_funcInt_1 (self, "bbia_compare_int");
	int res = bbia_compare_int_op (self, toCompare);

	if (isSigned == 0 && self->sign == 0) return res;
	else if (self->sign == isSigned) return (res == -1) ? 1 : -1;
	else if (self->sign > isSigned) return (res == 1) ? 1 : -1;
	return 0;
}

// @COPY

void bbia_copy_bbia (bbia * to, bbia * from) {
	nullPointer_funcVoid_2 (to, from, "bbia_copy_bbia");
	for (int curLvl = 0; curLvl <= BBIA_LEVEL_TOP; curLvl++)
		to->at[curLvl] = from->at[curLvl];
	to->sign = from->sign;
	to->usedLevelPos = from->usedLevelPos;
}

bbia * bbia_copy_new (bbia * from) {
	nullPointer_funcPointer_1 (from, "bbia_copy_new");
	bbia * to = bbia_new();
	bbia_copy_bbia (to, from);
	return to;
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

	self->usedLevelPos = (self->usedLevelPos == 0) ? 0 : self->usedLevelPos - 1;
	for (int lvl = BBIA_LEVEL_TOP; lvl > self->usedLevelPos; self->at[lvl] <<= value, lvl--) {
		bitMask[1] = self->at[lvl] & bitMask[0];
		stuaa_shiftr (bitMask+1, BBIA_INTEGER_SIZE-value);
		savedBits[lvl-1] = bitMask[1];
	}
	self->at[self->usedLevelPos] <<= value;

	for (int lvl = self->usedLevelPos; lvl < BBIA_LEVEL_TOP; lvl++)
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

	for (int lvl = self->usedLevelPos; lvl < BBIA_LEVEL_TOP; stuaa_shiftr (self->at+lvl,value), lvl++) {
		bitMask[1] = self->at[lvl] & bitMask[0];
		bitMask[1] <<= BBIA_INTEGER_SIZE-value;
		savedBits[lvl] = bitMask[1];
	}
	stuaa_shiftr (self->at+BBIA_LEVEL_TOP,value);

	for (int lvl = self->usedLevelPos; lvl < BBIA_LEVEL_TOP; lvl++)
		self->at[lvl+1] |= savedBits[lvl];

	if (self->at[self->usedLevelPos] == 0) self->usedLevelPos++;
}

void bbia_bits_or (bbia * first, bbia * second) {

	nullPointer_funcVoid_2 (first,second,"bbia_bits_or");
	first->usedLevelPos = (first->usedLevelPos >= second->usedLevelPos) ? first->usedLevelPos : second->usedLevelPos;

	for (int curLvl = first->usedLevelPos; curLvl <= BBIA_LEVEL_TOP; curLvl++)
		first->at[curLvl] |= second->at[curLvl];
}

bbia * bbia_bits_or_new (bbia * first, bbia * second) {

	nullPointer_funcPointer_2 (first,second,"bbia_bits_or_new");
	bbia * self = bbia_new();
	self->usedLevelPos = (first->usedLevelPos >= second->usedLevelPos) ? first->usedLevelPos : second->usedLevelPos;

	for (int curLvl = self->usedLevelPos; curLvl <= BBIA_LEVEL_TOP; curLvl++)
		self->at[curLvl] = first->at[curLvl] | second->at[curLvl];

	return self;
}

void bbia_bits_and (bbia * first, bbia * second) {

	nullPointer_funcVoid_2 (first,second,"bbia_bits_and");
	first->usedLevelPos = (first->usedLevelPos >= second->usedLevelPos) ? first->usedLevelPos : second->usedLevelPos;

	for (int curLvl = first->usedLevelPos; curLvl <= BBIA_LEVEL_TOP; curLvl++)
		first->at[curLvl] &= second->at[curLvl];
}

bbia * bbia_bits_and_new (bbia * first, bbia * second) {

	nullPointer_funcPointer_2 (first,second,"bbia_bits_and_new");
	bbia * self = bbia_new();
	self->usedLevelPos = (first->usedLevelPos >= second->usedLevelPos) ? first->usedLevelPos : second->usedLevelPos;

	for (int curLvl = self->usedLevelPos; curLvl <= BBIA_LEVEL_TOP; curLvl++)
		self->at[curLvl] = first->at[curLvl] & second->at[curLvl];

	return self;
}

void bbia_bits_flag_set (bbia * self, int num) {

	nullPointer_funcVoid_1 (self,"bbia_bits_flag_set");
	int lvl = BBIA_LEVEL_TOP - num / BBIA_INTEGER_SIZE;
	num %= BBIA_INTEGER_SIZE;
	if (num != 0)
		self->at[lvl] |= stuaa_bitflag (num);
	else
		self->at[lvl+1] |= stuaa_bitflag (BBIA_INTEGER_SIZE);
	bbia_check_usedLevelPosition (self);
}

void bbia_bits_flag_unset (bbia * self, int num) {

	nullPointer_funcVoid_1 (self,"bbia_bits_flag_unset");
	int lvl = BBIA_LEVEL_TOP - num / BBIA_INTEGER_SIZE;
	num %= BBIA_INTEGER_SIZE;
	if (num != 0)
		self->at[lvl] &= ~stuaa_bitflag (num);
	else
		self->at[lvl+1] &= ~stuaa_bitflag (BBIA_INTEGER_SIZE);
	bbia_check_usedLevelPosition (self);
}

void bbia_bits_flag_set_mult (bbia * self, int * numArray) {

	nullPointer_funcVoid_2 (self,numArray,"bbia_bits_flag_set_mult");
	while (numArray != NULL)
		bbia_bits_flag_set (self, *numArray++);
}

void bbia_bits_flag_unset_mult (bbia * self, int * numArray) {

	nullPointer_funcVoid_2 (self,numArray,"bbia_bits_flag_unset_mult");
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

	bbia_check_usedLevelPosition (self);
	return self;
}

bbia * bbia_bits_isFull () {

	bbia * self = bbia_new();
	for (int curLvl = BBIA_LEVEL_TOP; curLvl >= 0; curLvl--)
		self->at[curLvl] = BBIA_LEVEL_IS_FULL;
	self->usedLevelPos = 0;
	return self;
}

bbia * bbia_bits_isEmpty () {

	bbia * self = bbia_new();
	for (int curLvl = BBIA_LEVEL_TOP; curLvl >= 0; curLvl--)
		self->at[curLvl] = BBIA_LEVEL_IS_EMPTY;
	self->usedLevelPos = BBIA_LEVEL_TOP;
	return self;
}

bbia * bbia_bits_tillBit_isFull (int num) {

	bbia * self = bbia_new();
	int lvl = BBIA_LEVEL_TOP - num / BBIA_INTEGER_SIZE;
	num %= BBIA_INTEGER_SIZE;
	if (num == 0) lvl++;

	for (int curLvl = BBIA_LEVEL_TOP; curLvl > lvl; curLvl--)
		self->at[curLvl] = BBIA_LEVEL_IS_FULL;
	for (int curBit = 1; curBit <= num; curBit++)
		self->at[lvl] |= stuaa_bitflag (curBit);
	bbia_check_usedLevelPosition (self);
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
	bbia_check_usedLevelPosition (self);
	return self;
}

// @PRINT

void bbia_print_levelValue (bbia * self) {
	nullPointer_funcVoid_1 (self, "bbia_print_levelValue");

	char * tempMemory = NULL;
	for (int curLvl = 0; curLvl <= BBIA_LEVEL_TOP; curLvl++) {

		tempMemory = stuaa_toBase (self->at[curLvl], 2);
		printf ("%s_", tempMemory);

		if (tempMemory != NULL) free (tempMemory);
	}
	puts("");
}

void bbia_print_levelValue_dec (bbia * self) {
	nullPointer_funcVoid_1 (self, "bbia_print_levelValue_dec");

	for (int curLvl = 0; curLvl <= BBIA_LEVEL_TOP; curLvl++)
		printf ("%u_", self->at[curLvl]);
	puts("");
}
