#include "stuaa.h"
#include "bbia.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define throw(MSG) fprintf(stderr, "%s\n",MSG)

#define nullPointer_funcVoid_1(p, functionName) do { \
	if (p == NULL) { \
		fprintf (stderr, "null pointer in %s\n", functionName); \
		return; \
	} \
} while(0)
#define nullPointer_funcVoid_2(p1, p2, functionName) do{if(p1 == NULL || p2 == NULL){fprintf(stderr, "null pointer in %s\n", functionName);return;}}while(0)
#define nullPointer_funcVoid_3(p1, p2, p3, functionName) do{if(p1 == NULL || p2 == NULL || p3 == NULL){fprintf(stderr, "null pointer in %s\n", functionName);return;}}while(0)
#define nullPointer_funcPointer_1(p, functionName)do{if(p == NULL){fprintf(stderr, "null pointer in %s\n", functionName);return NULL;}} while(0)
#define nullPointer_funcPointer_2(p1, p2, functionName)do{if (p1 == NULL || p2 == NULL){fprintf(stderr, "null pointer in %s\n", functionName);return NULL;}}while(0)
#define nullPointer_funcPointer_3(p1, p2, p3, functionName)do{if (p1 == NULL || p2 == NULL || p3 == NULL){fprintf(stderr, "null pointer in %s\n", functionName);return NULL;}}while(0)
#define nullPointer_funcInt_1(p, functionName)do{if(p == NULL){fprintf(stderr, "null pointer in %s\n", functionName);return -42;}}while(0)
#define nullPointer_funcInt_2(p1, p2, functionName)do{if(p1 == NULL || p2 == NULL){fprintf(stderr, "null pointer in %s\n", functionName);return -42;}}while(0)
#define nullPointer_funcBool_1(p, functionName)do{if(p == NULL){fprintf(stderr, "null pointer in %s\n", functionName);return false;}}while(0)

typedef enum __DIVISION_OR_MOD { DIVISION = 2, MOD = 4 } DIVISION_OR_MOD;

struct __bbia {
	// some languages don`t have unsigned int
	signed int at[BBIA_LEVEL_COUNT];
	bool sign;
	signed int lvlButton;
};

// @CONSTRUCTOR

bbia * bbia_new (void) {
	bbia * self = calloc (sizeof(bbia), 1); if (self == NULL) abort();
	bbia_set_zero (self);
	return self;
}

bbia * bbia_new_fromSystemInteger (int integer, bool isSigned) {
	bbia * self = bbia_new();
	self->at[BBIA_LEVEL_TOP] = integer;
	self->sign = isSigned;
	return self;
}

void bbia_free (bbia * self) {
	nullPointer_funcVoid_1 (self, "bbia_free");
	free(self);
}

// @FIELDS

void bbia_sign_change (bbia * self) {
	nullPointer_funcVoid_1 (self, "bbia_sign_change");
	self->sign = (self->sign) ? false : true;
}

void bbia_sign_set (bbia * self, bool sign) {
	nullPointer_funcVoid_1 (self, "bbia_sign_set");
	self->sign = sign;
}

bool bbia_sign_check (bbia * self) {
	nullPointer_funcBool_1 (self, "bbia_sign_check");
	return self->sign;
}

void bbia_lvlButton_conf (bbia * self) {
	nullPointer_funcVoid_1 (self, "bbia_lvlButton_conf");
	for (int curLvl = 0; stuaa_compare (curLvl, BBIA_LEVEL_TOP) == -1; curLvl++)
		if (self->at[curLvl] != 0) {
			self->lvlButton = curLvl;
			return;
		}
	self->lvlButton = BBIA_LEVEL_TOP;
}

void bbia_lvlButton_set (bbia * self, int lvl) {
	nullPointer_funcVoid_1 (self, "bbia_lvlButton_set");
	self->lvlButton = lvl;
}

int bbia_lvlButton_get (bbia * self) {
	nullPointer_funcInt_1 (self, "bbia_lvlButton_get");
	return self->lvlButton;
}

// @SETERS

inline static void bbia_setters_main_op (bbia * self, int val, bool sign) {
	if (val != BBIA_LEVEL_IS_EMPTY) {
		for (int i = 0; stuaa_compare (i, BBIA_LEVEL_TOP) != 1; i++) self->at[i] = val;
		self->sign = sign;
		self->lvlButton = 0;
	}
	else {
		for (int i = 0; stuaa_compare (i, BBIA_LEVEL_TOP) != 1; i++) self->at[i] = BBIA_LEVEL_IS_EMPTY;
		self->sign = 0;
		self->lvlButton = BBIA_LEVEL_TOP;
	}
}
void bbia_set_zero (bbia * self) {bbia_setters_main_op (self, BBIA_LEVEL_IS_EMPTY, false);}
void bbia_set_min (bbia * self) {bbia_setters_main_op (self, BBIA_LEVEL_IS_FULL, true);}
void bbia_set_max (bbia * self) {bbia_setters_main_op (self, BBIA_LEVEL_IS_FULL, false);}
void bbia_set_value (bbia * self, int value) {bbia_setters_main_op (self, value, false);}
void bbia_set_value_signed (bbia * self, int value) {bbia_setters_main_op (self, value, true);}

void bbia_set_value_fromLevel (bbia * self, int level, int value) {
	nullPointer_funcVoid_1 (self, "bbia_set_value_fromLevel");

	for (int curLvl = 0; stuaa_compare (curLvl, level) != 1; curLvl++)
		self->at[curLvl] = value;
	self->lvlButton = 0; if (value == BBIA_LEVEL_IS_EMPTY) bbia_lvlButton_conf (self);
}

void bbia_set_value_toLevel (bbia * self, int level, int value) {
	nullPointer_funcVoid_1 (self, "bbia_set_value_toLevel");

	for (int curLvl = level; stuaa_compare (curLvl, BBIA_LEVEL_TOP) != 1; curLvl++)
		self->at[curLvl] = value;
	self->lvlButton = level; if (value == BBIA_LEVEL_IS_EMPTY) bbia_lvlButton_conf (self);
}

void bbia_set_systemInteger (bbia * self, int integer, bool isSigned) {
	nullPointer_funcVoid_1 (self, "bbia_set_systemInteger");
	bbia_set_zero (self);
	self->at[BBIA_LEVEL_TOP] = integer;
	self->sign = isSigned;
}

int bbia_at_get (bbia * self, int index) {
	nullPointer_funcInt_1 (self, "bbia_at_get");
	return self->at[index];
}

void bbia_at_set (bbia * self, int index, int value) {
	nullPointer_funcVoid_1 (self, "bbia_at_set");
	self->at[index] = value;
	if (self->lvlButton > index) self->lvlButton = index;
}


// @CHECK

bool bbia_check_is_integer (bbia * self, int integer) {
	nullPointer_funcBool_1 (self, "bbia_check_is_integer");
	if (stuaa_compare (self->lvlButton, BBIA_LEVEL_TOP) == -1) return false;

	return (self->at[BBIA_LEVEL_TOP] == integer) ? true : false;
}

bool bbia_check_is_zero (bbia * self) {
	return bbia_check_is_integer (self, 0);
}

bool bbia_check_is_one (bbia * self) {
	return bbia_check_is_integer (self, 1);
}

bool bbia_check_is_systemInteger (bbia * self) {
	nullPointer_funcBool_1 (self, "bbia_check_is_systemInteger");
	return (self->lvlButton == BBIA_LEVEL_TOP) ? true : false;
}

// @COMPARE

int bbia_compare_bbia_unsigned (bbia * a, bbia * b) {
	int curCompare = 0;
	int curLvl = (stuaa_compare (a->lvlButton, b->lvlButton) != 1) ? a->lvlButton : b->lvlButton;
	for (; stuaa_compare (curLvl, BBIA_LEVEL_TOP) != 1; curLvl++) {
		curCompare = stuaa_compare (a->at[curLvl], b->at[curLvl]);
		if (curCompare != 0)
			return curCompare;
	}
	return 0;
}

int bbia_compare_bbia (bbia * a, bbia * b) {
	nullPointer_funcInt_2 (a, b, "bbia_compare_bbia");
	int res = bbia_compare_bbia_unsigned (a,b);

	if (a->sign == b->sign) return res;
	else if (a->sign == true) return -1;
	else if (b->sign == true) return 1;
	return 0;
}

int bbia_compare_int_unsigned (bbia * self, int toCompare) {
	if (stuaa_compare (self->lvlButton, BBIA_LEVEL_TOP) == -1) return 1;
	return stuaa_compare (self->at[BBIA_LEVEL_TOP], toCompare);
}

int bbia_compare_int (bbia * self, int toCompare, bool isSigned) {
	nullPointer_funcInt_1 (self, "bbia_compare_int");
	int res = bbia_compare_int_unsigned (self, toCompare);

	if (self->sign == isSigned) return res;
	else if (self->sign == true) return -1;
	else if (isSigned == true) return 1;
	return 0;
}

// @COPY

void bbia_copy_bbia (bbia * to, bbia * from) {
	nullPointer_funcVoid_2 (to, from, "bbia_copy_bbia");
	for (int curLvl = 0; stuaa_compare (curLvl, BBIA_LEVEL_TOP) != 1; curLvl++)
		to->at[curLvl] = from->at[curLvl];
	to->sign = from->sign;
	to->lvlButton = from->lvlButton;
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

	int savedBits [BBIA_LEVEL_TOP];
	int bitMask[2] = {0,0};
	for (int curBit = BBIA_INTEGER_SIZE-value+1; stuaa_compare (curBit, BBIA_INTEGER_SIZE) != 1; curBit++)
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
	self->lvlButton = (self->lvlButton == 0) ? 0 : self->lvlButton - 1;
	for (int lvl = BBIA_LEVEL_TOP; stuaa_compare (lvl, self->lvlButton) == 1; self->at[lvl] <<= value, lvl--) {
		bitMask[1] = self->at[lvl] & bitMask[0];
		stuaa_shiftr (bitMask+1, BBIA_INTEGER_SIZE-value);
		savedBits[lvl-1] = bitMask[1];
	}
	self->at[self->lvlButton] <<= value;

	for (int lvl = self->lvlButton; stuaa_compare (lvl, BBIA_LEVEL_TOP) == -1; lvl++)
		self->at[lvl] |= savedBits[lvl];
}

void bbia_bits_shift_right (bbia * self, int value) {
	nullPointer_funcVoid_1 (self, "bbia_bits_shift_right");

	int savedBits [BBIA_LEVEL_TOP];
	int bitMask[2] = {0,0};
	for (int curBit = 1; stuaa_compare (curBit, value) != 1; curBit++)
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
	for (int lvl = self->lvlButton; stuaa_compare (lvl, BBIA_LEVEL_TOP) == -1; stuaa_shiftr (self->at+lvl,value), lvl++) {
		bitMask[1] = self->at[lvl] & bitMask[0];
		bitMask[1] <<= BBIA_INTEGER_SIZE-value;
		savedBits[lvl] = bitMask[1];
	}
	stuaa_shiftr (self->at+BBIA_LEVEL_TOP,value);

	for (int lvl = self->lvlButton; stuaa_compare (lvl, BBIA_LEVEL_TOP) == -1; lvl++)
		self->at[lvl+1] |= savedBits[lvl];
	if (self->at[self->lvlButton] == BBIA_LEVEL_IS_EMPTY) self->lvlButton++;
}

void bbia_bits_or (bbia * first, bbia * second) {
	nullPointer_funcVoid_2 (first,second,"bbia_bits_or");
	first->lvlButton = (stuaa_compare (first->lvlButton, second->lvlButton) != -1) ? first->lvlButton : second->lvlButton;

	for (int curLvl = first->lvlButton; stuaa_compare (curLvl, BBIA_LEVEL_TOP) != 1; curLvl++)
		first->at[curLvl] |= second->at[curLvl];
}

bbia * bbia_bits_or_new (bbia * first, bbia * second) {
	nullPointer_funcPointer_2 (first,second,"bbia_bits_or_new");
	bbia * self = bbia_new();
	self->lvlButton = (stuaa_compare (first->lvlButton, second->lvlButton) != -1) ? first->lvlButton : second->lvlButton;

	for (int curLvl = self->lvlButton; stuaa_compare (curLvl, BBIA_LEVEL_TOP) != 1; curLvl++)
		self->at[curLvl] = first->at[curLvl] | second->at[curLvl];
	return self;
}

void bbia_bits_and (bbia * first, bbia * second) {
	nullPointer_funcVoid_2 (first,second,"bbia_bits_and");
	first->lvlButton = (stuaa_compare (first->lvlButton, second->lvlButton) != -1) ? first->lvlButton : second->lvlButton;

	for (int curLvl = first->lvlButton; stuaa_compare (curLvl, BBIA_LEVEL_TOP) != 1; curLvl++)
		first->at[curLvl] &= second->at[curLvl];
}

bbia * bbia_bits_and_new (bbia * first, bbia * second) {
	nullPointer_funcPointer_2 (first,second,"bbia_bits_and_new");
	bbia * self = bbia_new();
	self->lvlButton = (stuaa_compare (first->lvlButton, second->lvlButton) != -1) ? first->lvlButton : second->lvlButton;

	for (int curLvl = self->lvlButton; stuaa_compare (curLvl, BBIA_LEVEL_TOP) != 1; curLvl++)
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
	bbia_lvlButton_conf (self);
}

void bbia_bits_flag_unset (bbia * self, int num) {
	nullPointer_funcVoid_1 (self,"bbia_bits_flag_unset");
	int lvl = BBIA_LEVEL_TOP - num / BBIA_INTEGER_SIZE;
	num %= BBIA_INTEGER_SIZE;
	if (num != 0)
		self->at[lvl] &= ~stuaa_bitflag (num);
	else
		self->at[lvl+1] &= ~stuaa_bitflag (BBIA_INTEGER_SIZE);
	bbia_lvlButton_conf (self);
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

	bbia_lvlButton_conf (self);
	return self;
}

bbia * bbia_bits_isFull () {
	bbia * self = bbia_new();
	for (int curLvl = 0; stuaa_compare (curLvl, BBIA_LEVEL_TOP) != 1; curLvl++)
		self->at[curLvl] = BBIA_LEVEL_IS_FULL;
	self->lvlButton = 0;
	return self;
}

bbia * bbia_bits_isEmpty () {
	bbia * self = bbia_new();
	for (int curLvl = 0; stuaa_compare (curLvl, BBIA_LEVEL_TOP) != 1; curLvl++)
		self->at[curLvl] = BBIA_LEVEL_IS_EMPTY;
	self->lvlButton = BBIA_LEVEL_TOP;
	return self;
}

bbia * bbia_bits_tillBit_isFull (int num) {
	bbia * self = bbia_new();
	int lvl = BBIA_LEVEL_TOP - num / BBIA_INTEGER_SIZE;
	num %= BBIA_INTEGER_SIZE;
	if (num == 0) lvl++;

	for (int curLvl = lvl + 1; stuaa_compare (curLvl, BBIA_LEVEL_TOP) != 1; curLvl--)
		self->at[curLvl] = BBIA_LEVEL_IS_FULL;
	for (int curBit = 1; stuaa_compare (curBit, num) != 1; curBit++)
		self->at[lvl] |= stuaa_bitflag (curBit);
	bbia_lvlButton_conf (self);
	return self;
}

bbia * bbia_bits_tillBit_isEmpty (int num) {
	bbia * self = bbia_new();
	int lvl = BBIA_LEVEL_TOP - num / BBIA_INTEGER_SIZE;
	num %= BBIA_INTEGER_SIZE;
	if (num == 0) lvl++;

	for (int curLvl = lvl + 1; stuaa_compare (curLvl, BBIA_LEVEL_TOP) != 1; curLvl--)
		self->at[curLvl] = BBIA_LEVEL_IS_EMPTY;
	for (int curBit = 1; stuaa_compare (curBit, num) != 1; curBit++)
		self->at[lvl] &= ~(stuaa_bitflag (curBit));
	bbia_lvlButton_conf (self);
	return self;
}

// bbia and bbia ADD/SUB private module operations

void bbia_add_bbia_op (bbia * to, bbia * from) {
	// if from is system integer and out of bounders not possible then try simple language addition
	if (bbia_check_is_systemInteger (from) && to->at[0] < BBIA_LEVEL_IS_FULL) {
		bbia_add_int_level (to, from->at[BBIA_LEVEL_TOP], BBIA_LEVEL_TOP);
		return;
	}
	else {
		to->lvlButton = (to->lvlButton <= from->lvlButton) ? to->lvlButton : from->lvlButton;
		int level = BBIA_LEVEL_TOP;
		register int outBit = 0;
		register int curBit = 0;

		// standart addition algorithm for binary N-bit integer
		for (; (level >= to->lvlButton) || (outBit != 0 && level > 0); level--)
		for (int bitPos = 1; bitPos <= BBIA_INTEGER_SIZE; bitPos++) {
			// if nBit of to is enabled
			curBit = (to->at[level] & stuaa_bitflag (bitPos)) ? 1 : 0;
			// if nBit of from is enabled
			if (from->at[level] & stuaa_bitflag (bitPos)) curBit++;

			if (outBit != 0) { outBit--; curBit++; }
			if (curBit > 1) { outBit++; curBit -= 2; }

			// enable ot disable bit in to
			if (curBit == 1) to->at[level] |= stuaa_bitflag (bitPos);
			else to->at[level] &= ~(stuaa_bitflag (bitPos));
		}
		// OUT OF BOUNDERS
		if (level == 0 && outBit != 0) {
			bbia_set_zero (to);
			level = BBIA_LEVEL_TOP;
			for (; outBit != 0 && level > 0; level--)
			for (int bitPos = 1; bitPos <= BBIA_INTEGER_SIZE; outBit--, bitPos++)
			to->at[level] |= stuaa_bitflag (bitPos);
			bbia_lvlButton_conf (to);
			bbia_sign_change (to);
		}
		else if (to->at[level] != 0) to->lvlButton = level;
	}
}

void bbia_sub_bbia_op (bbia * from, bbia * subtrahend) {
	// if subtrahend is system integer and out of bounders not possible then try simple language difference
	if (bbia_check_is_systemInteger (subtrahend) == 1 && bbia_check_is_systemInteger (from) == 1
	&& stuaa_outofbounders_min (from->at[BBIA_LEVEL_TOP], subtrahend->at[BBIA_LEVEL_TOP]) != 1) {
		bbia_sub_int_level (from, subtrahend->at[BBIA_LEVEL_TOP], BBIA_LEVEL_TOP);
		return;
	}
	// OUT OF BOUNDERS
	else if (bbia_compare_bbia_unsigned (subtrahend, from) == 1) {
		bbia * temp = bbia_copy_new (subtrahend);
		bbia_sub_bbia_op (temp, from);
		bbia_sign_set (temp, (from->sign == false) ? true : false);
		bbia_copy_bbia (from, temp);
		bbia_free (temp);
		return;
	}
	else {
		int bitPos = 1;
		int level = BBIA_LEVEL_TOP;
		register int outBit = 0;
		register int curBitSecond = 0;
		register int curBitFirst = 0;

		for (; level >= from->lvlButton; level--)
		for (bitPos = 1; bitPos <= BBIA_INTEGER_SIZE; bitPos++) {
			curBitFirst = (from->at[level] & stuaa_bitflag (bitPos)) ? 1 : 0;
			curBitSecond = (subtrahend->at[level] & stuaa_bitflag (bitPos)) ? 1 : 0;

			if (curBitFirst == curBitSecond) {
				if (outBit == 0) from->at[level] &= ~(stuaa_bitflag (bitPos));
				else {
					from->at[level] |= stuaa_bitflag (bitPos);
					outBit = 0;
				}
			}
			else if (curBitSecond == 1) {
				if (outBit == 1) from->at[level] &= ~(stuaa_bitflag (bitPos));
				else {
					from->at[level] |= stuaa_bitflag (bitPos);
					outBit = curBitSecond;
				}
			}
			else if (outBit != 0) {
				from->at[level] &= ~(stuaa_bitflag (bitPos));
				outBit = 0;
			}
		}
		bbia_lvlButton_conf (from);
	}
}

// @ADD

void bbia_add_int_levelOut (bbia * self, int integer, int fromLvl, int prevLvl) {
	if (prevLvl != 0) {
		if (stuaa_outofbounders_max(self->at[prevLvl-1],1) == 1) {
			bbia_add_int_levelOut (self, integer, fromLvl, prevLvl-1);
			return;
		}
		else
			self->at[prevLvl-1]++;
	}
	else if (prevLvl == 0) {
		bbia * temp = bbia_new();
		temp->at[fromLvl] = integer;
		temp->lvlButton = fromLvl;
		bbia_add_bbia_op (self, temp);
		bbia_free (temp);
		return;
	}

	for (int curLvl = prevLvl; curLvl < fromLvl; curLvl++)
		self->at[curLvl] = BBIA_LEVEL_IS_EMPTY;
	// x = x + y
	// the number needed to overflow is z = FULL - y + 1
	// the value to set is x = EMPTY + x - z
	self->at[fromLvl] = BBIA_LEVEL_IS_EMPTY + self->at[fromLvl] - (BBIA_LEVEL_IS_FULL - integer + 1);
}

void bbia_add_int_level (bbia * self, int integer, int level) {
	if (stuaa_outofbounders_max (self->at[level], integer) == 0)
		self->at[level] += integer;
	else
		bbia_add_int_levelOut (self, integer, level, level);
}

void bbia_add_int (bbia * self, int integer) {
	nullPointer_funcVoid_1 (self, "bbia_add_int");
	if (self->sign == false)
		bbia_add_int_level (self, integer, BBIA_LEVEL_TOP);
	else if (self->sign == true)
		bbia_sub_int_level (self, integer, BBIA_LEVEL_TOP);
}

void bbia_add_bbia (bbia * first, bbia * second) {
	nullPointer_funcVoid_2 (first, second, "bbia_add_bbia");
	if (first->sign == second->sign)
		bbia_add_bbia_op (first, second);
	else
		bbia_sub_bbia_op (first, second);
}

void bbia_add_bbia_to (bbia * to, bbia * first, bbia * second) {
	nullPointer_funcVoid_3 (to, first, second, "bbia_add_bbia_to");
	bbia_copy_bbia (to, first);
	if (first->sign == second->sign)
		bbia_add_bbia_op (to, second);
	else
		bbia_sub_bbia_op (to, second);
}

bbia * bbia_add_bbia_new (bbia * first, bbia * second) {
	nullPointer_funcPointer_2 (first, second, "bbia_add_bbia_new");
	bbia * self = bbia_copy_new(first);
	if (first->sign == second->sign)
		bbia_add_bbia_op (self, second);
	else
		bbia_sub_bbia_op (self, second);
	return self;
}

// SUBTRAHEND

void bbia_sub_int_levelOut (bbia * self, int integer, int fromLvl, int prevLvl) {
	if (prevLvl != 0) {
		if (stuaa_outofbounders_min(self->at[prevLvl-1],1) == 1) {
			bbia_sub_int_levelOut (self, integer, fromLvl, prevLvl-1);
			return;
		}
		else
			self->at[prevLvl-1]--;
	}
	else if (prevLvl == 0) {
		bbia * temp = bbia_new();
		temp->at[fromLvl] = integer;
		temp->lvlButton = fromLvl;
		bbia_sub_bbia_op (self, temp);
		bbia_free (temp);
		return;
	}

	for (int curLvl = prevLvl; curLvl < fromLvl; curLvl++)
		self->at[curLvl] = BBIA_LEVEL_IS_FULL;
	// x = x - y
	// the number needed to overflow is z = EMPTY + y - 1
	// the value to set x = FULL - x + z
	self->at[fromLvl] = BBIA_LEVEL_IS_FULL - self->at[fromLvl] + (BBIA_LEVEL_IS_EMPTY + integer - 1);
	// @TODO CHECK
	for (int curLvl = prevLvl+1; curLvl <= BBIA_LEVEL_TOP; curLvl++)
		self->at[curLvl] = BBIA_LEVEL_IS_FULL - self->at[curLvl] + 1;
}

void bbia_sub_int_level (bbia * self, int integer, int level) {
	if (stuaa_outofbounders_min (self->at[level], integer) == 0)
		self->at[level] -= integer;
	else
		bbia_sub_int_levelOut (self, integer, level, level);
}

void bbia_sub_int (bbia * self, int integer) {
	nullPointer_funcVoid_1 (self, "bbia_sub_int");
	if (self->sign == false)
		bbia_sub_int_level (self, integer, BBIA_LEVEL_TOP);
	else if (self->sign == true)
		bbia_add_int_level (self, integer, BBIA_LEVEL_TOP);
}

void bbia_sub_bbia (bbia * first, bbia * second) {
	nullPointer_funcVoid_2 (first, second, "bbia_sub_bbia");
	if (first->sign == second->sign)
		bbia_sub_bbia_op (first, second);
	else
		bbia_add_bbia_op (first, second);
}

void bbia_sub_bbia_to (bbia * to, bbia * first, bbia * second) {
	nullPointer_funcVoid_3 (to, first, second, "bbia_sub_bbia_to");
	bbia_copy_bbia (to, first);
	if (first->sign == second->sign)
		bbia_sub_bbia_op (to, second);
	else
		bbia_add_bbia_op (to, second);
}

bbia * bbia_sub_bbia_new (bbia * first, bbia * second) {
	nullPointer_funcPointer_2 (first, second, "bbia_sub_bbia_new");
	bbia * self = bbia_copy_new(first);
	if (first->sign == second->sign)
		bbia_sub_bbia_op (self, second);
	else
		bbia_add_bbia_op (self, second);
	return self;
}

// @MULTIPLICATION

bbia * bbia_multiplicationByBitAnd_operation (bbia * self, bbia * temp) {

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
			bbia_add_bbia (result, temp);

		bbia_bits_shift_left(temp, 1);
	}

	bbia_lvlButton_conf (result);
	return result;
}

bbia * bbia_mult_int_new (bbia * self, int integer) {
	nullPointer_funcPointer_1 (self, "bbia_mult_int_new");
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

bbia * bbia_mult_bbia_new (bbia * first, bbia * second) {
	nullPointer_funcPointer_2 (first, second, "bbia_mult_bbia_new");
	if (first == second)
		return bbia_pow_new (first, 2);

	bbia * temp = bbia_copy_new (second);
	bbia * result = bbia_multiplicationByBitAnd_operation (first, temp);
	bbia_free (temp);

	if ((first->sign != second->sign) || (first->sign == true && second->sign == true)) bbia_sign_change (result);
	return result;
}

void bbia_mult_bbia (bbia * to, bbia * second) {
	if (to == second) {
		bbia_pow (to, 2);
		return;
	}
	int toChangeSign = ((to->sign != second->sign) || (to->sign == true && second->sign == true)) ? true : false;
	bbia * res = bbia_multiplicationByBitAnd_operation (second, to);

	bbia_copy_bbia (to, res);
	bbia_free (res);
	if (toChangeSign == true) bbia_sign_change (to);
}

// @POWER

void bbia_pow (bbia * self, int power) {
	nullPointer_funcVoid_1 (self, "bbia_pow");
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

// @DIVISION

void bbia_divisionBy_operation (bbia * self, bbia * division, DIVISION_OR_MOD flag) {

	bbia * mod = bbia_copy_new(self);

	// set result to zero and work with it like with current multiplication of division
	bbia_set_value (self, 0);
	bbia * currentMultiplierOfDivision = self; // sugar
	bbia_set_zero (currentMultiplierOfDivision);
	currentMultiplierOfDivision->at[BBIA_LEVEL_TOP] = 2;

	// create temporary to store what must be subtrahended from mod
	bbia * currentDifference = bbia_copy_new (division);

	for (; bbia_compare_bbia (mod, currentDifference) == 1;
	       bbia_add_int (currentMultiplierOfDivision, 1) /*analog for i++)*/)
	{
		bbia_copy_bbia (currentDifference, division);
		bbia_mult_bbia (currentDifference, currentMultiplierOfDivision);
	}

	bbia_sub_int (currentMultiplierOfDivision, 1); // analog for i--
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
		bbia_sub_bbia (mod, currentDifference);
		bbia_free(currentDifference);

		bbia_copy_bbia (self, mod);
		bbia_free (mod);
	}
}

void bbia_div_bbia (bbia * divided, bbia * division) {
	nullPointer_funcVoid_2 (divided, division, "bbia_div_bbia");
	bbia_divisionBy_operation (divided, division, DIVISION);
	bbia_lvlButton_conf (divided);
}

bbia * bbia_div_bbia_new (bbia * divided, bbia * division) {
	bbia * res = bbia_copy_new (divided);
	bbia_divisionBy_operation (res, division, DIVISION);
	bbia_lvlButton_conf (res);
	return res;
}

void bbia_div_int (bbia * self, int integer) {
	nullPointer_funcVoid_1 (self, "bbia_div_int");
	if (integer == BBIA_LEVEL_IS_EMPTY) {
		throw("division by zero");
		return;
	}
	bbia * division = bbia_new();
	division->at[BBIA_LEVEL_TOP] = integer;

	bbia_divisionBy_operation (self, division, DIVISION);
	bbia_lvlButton_conf (self);
	bbia_free (division);
}

bbia * bbia_div_int_new (bbia * self, int integer) {
	bbia * res = bbia_copy_new (self);
	bbia_div_int (res, integer);
	bbia_lvlButton_conf (res);
	return res;
}

// @MOD

void bbia_mod_bbia (bbia * divided, bbia * division) {
	nullPointer_funcVoid_2 (divided, division, "bbia_mod_bbia");
	bbia_divisionBy_operation (divided, division, MOD);
	bbia_lvlButton_conf (divided);
}

bbia * bbia_mod_bbia_new (bbia * divided, bbia * division) {
	bbia * res = bbia_copy_new (divided);
	bbia_divisionBy_operation (res, division, MOD);
	bbia_lvlButton_conf (res);
	return res;
}

void bbia_mod_int (bbia * self, int integer) {
	nullPointer_funcVoid_1 (self, "bbia_mod_int");
	bbia * division = bbia_new();
	division->at[BBIA_LEVEL_TOP] = integer;

	bbia_divisionBy_operation (self, division, MOD);
	bbia_lvlButton_conf (self);
	bbia_free (division);
}

bbia * bbia_mod_int_new (bbia * self, int integer) {
	bbia * res = bbia_copy_new (self);
	bbia_mod_int (res, integer);
	bbia_lvlButton_conf (res);
	return res;
}

// LOGARITHM

void bbia_log_int (int base, int isSigned, bbia * self) {
	nullPointer_funcVoid_1 (self, "bbia_log_int");

	if (bbia_check_is_one (self)) {
		bbia_set_zero (self);
		return;
	}
	// bbia can`t use non-integer power
	if (bbia_compare_int_unsigned (self, base) == -1) {
		return;
	}
	if (bbia_check_is_zero (self) || base == 0) {
		return;
	}

	int curPower = 1;
	bbia * compare = bbia_new_fromSystemInteger (base, isSigned);
	while (bbia_compare_bbia_unsigned (compare, self) == -1) {
		bbia_set_systemInteger (compare, base, isSigned);
		bbia_pow (compare, ++curPower);
	}
	bbia_set_systemInteger (self, curPower, (isSigned == true && curPower % 2 != 0) ? 1 : 0);
	bbia_free (compare);
}

bbia * bbia_log_int_new (int base, int isSigned, bbia * self) {
	nullPointer_funcPointer_1 (self, "bbia_log_int_new");


	if (bbia_check_is_one (self)) {
		bbia_set_zero (self);
		return NULL;
	}
	// bbia can`t use non-integer power
	if (bbia_compare_int_unsigned (self, base) == -1) {
		return NULL;
	}
	if (bbia_check_is_zero (self) || base == 0) {
		return NULL;
	}

	int curPower = 1;
	bbia * compare = bbia_new_fromSystemInteger (base, isSigned);
	while (bbia_compare_bbia_unsigned (compare, self) < 0) {
		bbia_set_systemInteger (compare, base, isSigned);
		bbia_pow (compare, ++curPower);
	}
	bbia_set_systemInteger (compare, curPower, (isSigned == true && curPower % 2 != 0) ? 1 : 0);
	return compare;
}

void bbia_log_bbia (bbia * base, bbia * self) {
	nullPointer_funcVoid_2 (self, base, "bbia_log_int");

	if (bbia_check_is_one (self)) {
		bbia_set_zero (self);
		return;
	}
	// bbia can`t use non-integer power
	if (bbia_compare_bbia_unsigned (self, base) == -1) {
		return;
	}
	if (bbia_check_is_zero (self) || bbia_check_is_zero (base)) {
		return;
	}

	int curPower = 1;
	bbia * compare = bbia_copy_new (base);
	while (bbia_compare_bbia_unsigned (compare, self) < 0) {
		bbia_copy_bbia (compare, base);
		bbia_pow (compare, ++curPower);
	}
	bbia_set_systemInteger (self, curPower, (base->sign == true && curPower % 2 != 0) ? 1 : 0);
	bbia_free (compare);
}

bbia * bbia_log_bbia_new (bbia * base, bbia * self) {
	nullPointer_funcPointer_2 (self, base, "bbia_log_int_new");

	if (bbia_check_is_one (self)) {
		bbia_set_zero (self);
		return NULL;
	}
	// bbia can`t use non-integer power
	if (bbia_compare_bbia_unsigned (self, base) == -1) {
		return NULL;
	}
	if (bbia_check_is_zero (self) || bbia_check_is_zero (base)) {
		return NULL;
	}

	int curPower = 1;
	bbia * compare = bbia_copy_new (base);
	while (bbia_compare_bbia_unsigned (compare, self) < 0) {
		bbia_copy_bbia (compare, base);
		bbia_pow (compare, ++curPower);
	}
	bbia_set_systemInteger (compare, curPower, (base->sign == true && curPower % 2 != 0) ? 1 : 0);
	return compare;
}

// SQRT

void bbia_sqrt (bbia * self) {
	nullPointer_funcVoid_1 (self, "bbia_sqrt");

	bbia * compare = bbia_copy_new (self);
	bbia * temp = bbia_pow_new (self, 2);

	while (bbia_compare_bbia_unsigned (temp, compare) == 1 ) {
		bbia_free (temp);
		// x / r
		temp = bbia_div_bbia_new (compare,self);
		// r + x / r
		bbia_add_bbia (self, temp);
		bbia_free (temp);
		// (r + x / r) / 2
		bbia_bits_shift_right (self, 1);
		// temp = r*r
		bbia * temp = bbia_pow_new (self, 2);
	}
	bbia_free (temp);
	bbia_free (compare);
}

bbia * bbia_sqrt_new (bbia * self) {
	nullPointer_funcPointer_1 (self, "bbia_sqrt_new");

	bbia * compare = bbia_copy_new (self);
	bbia * temp = bbia_pow_new (compare, 2);

	while (bbia_compare_bbia_unsigned (temp, self) == 1 ) {
		bbia_free (temp);
		// x / r
		temp = bbia_div_bbia_new (self,compare);
		// r + x / r
		bbia_add_bbia (compare, temp);
		bbia_free (temp);
		// (r + x / r) / 2
		bbia_bits_shift_right (compare, 1);
		// temp = r*r
		bbia * temp = bbia_pow_new (compare, 2);
	}
	bbia_free (temp);
	return compare;
}

// @PRINT

void bbia_print_levelValue (bbia * self) {
	nullPointer_funcVoid_1 (self, "bbia_print_levelValue");
	char * tempMemory = NULL;
	printf ("%c|", (self->sign == 1) ? '-' : '+');
	for (int curLvl = 0; curLvl <= BBIA_LEVEL_TOP; curLvl++) {
		tempMemory = stuaa_toBase (self->at[curLvl], 2);
		printf ("%s_", tempMemory);
		if (tempMemory != NULL) free (tempMemory);
	}
	puts("");
}

void bbia_print_levelValue_dec (bbia * self) {
	nullPointer_funcVoid_1 (self, "bbia_print_levelValue_dec");
	printf ("%c|", (self->sign == true) ? '-' : '+');
	for (int curLvl = 0; curLvl <= BBIA_LEVEL_TOP; curLvl++)
		printf ("%u_", self->at[curLvl]);
	puts("");
}

// @INBASE

static const char * numerics = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ+/,"; //base64

char * bbia_base_to (bbia * self, int base) {
	nullPointer_funcPointer_1 (self, "bbia_base_to");

	int powerOfTwo = stuaa_isPowerOfTwo (base);
	if (powerOfTwo != -2) {

		const int size =
		1 // for sign in zero position
		+ log2 (self->at[self->lvlButton]) / log2 (base)
		+ ceil(BBIA_INTEGER_SIZE/powerOfTwo) * (BBIA_LEVEL_TOP-self->lvlButton);

		char * reverse = calloc (sizeof(char *), size + 1);

		for (
			int position = size,
			curLvl = BBIA_LEVEL_TOP,
			curBit = 0,
			curBitInTwo = 0,
			curDigit = 0;

			position > 0;

			position--
		)
		{
			for (curBitInTwo = 1; curBitInTwo <= powerOfTwo; curBitInTwo++) {

				if (curBit == BBIA_INTEGER_SIZE) {
					curBit = 0;
					curLvl--;
				}

				curDigit |=
				  (stuaa_bitflag (++curBit) & self->at[curLvl])
				? stuaa_bitflag (curBitInTwo) : 0;
			}

			reverse[position] = numerics[curDigit];
			curDigit = 0;
		}

		reverse[0] = (self->sign == false) ? '+' : '-';
		return reverse;
	}

	return NULL;
}

bbia * bbia_base_from (char * str, int base) {
	nullPointer_funcPointer_1 (str, "bbia_base_from");

	int powerOfTwo = stuaa_isPowerOfTwo (base);
	if (powerOfTwo != -2) {

		bbia * result = bbia_new();
		result->sign = str[0] == '-' ? true : false;
		int size = (int) strlen(str);

		for (
			int position = size-1,
			curLvl = BBIA_LEVEL_TOP,
			curBit = 0,
			curBitInTwo = 0,
			curDigit = 0;

			position > 0;

			position--
		)
		{
			curDigit = strchr (numerics, str[position]) - numerics;

			for (curBitInTwo = 1; curBitInTwo <= powerOfTwo; curBitInTwo++) {

				if (curBit == BBIA_INTEGER_SIZE) {
					curBit = 0;
					curLvl--;
				}


				if (stuaa_bitflag (curBitInTwo) & curDigit)
					result->at[curLvl] |= stuaa_bitflag (++curBit);
				else
					++curBit;
			}
		}

		return result;
	}

	return NULL;
}
