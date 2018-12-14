#include "stuaa.h"
#include "bbia.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <math.h>

#define throw(MSG) fprintf(stderr, "%s\n", MSG)

static const char * numerics =
"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz+/,"; //base64
// "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz!#$&()*+,;./:;<=>?@[]^-_'{|}~";

static inline double log_base(double base, double num) {
	return log2(num) / log2(base);
}

int stuaa_bitflag (int num) {

	if ( !(num >= 0 && num <= BBIA_INTEGER_SIZE) )
		return 0;

	switch (num) {
		case 1 : return 0x1;
		case 2 : return 0x2;
		case 3 : return 0x4;
		case 4 : return 0x8;
		case 5 : return 0x10;
		case 6 : return 0x20;
		case 7 : return 0x40;
		case 8 : return 0x80;
		case 9 : return 0x100;
		case 10 : return 0x200;
		case 11 : return 0x400;
		case 12 : return 0x800;
		case 13 : return 0x1000;
		case 14 : return 0x2000;
		case 15 : return 0x4000;
		case 16 : return 0x8000;
		case 17 : return 0x10000;
		case 18 : return 0x20000;
		case 19 : return 0x40000;
		case 20 : return 0x80000;
		case 21 : return 0x100000;
		case 22 : return 0x200000;
		case 23 : return 0x400000;
		case 24 : return 0x800000;
		case 25 : return 0x1000000;
		case 26 : return 0x2000000;
		case 27 : return 0x4000000;
		case 28 : return 0x8000000;
		case 29 : return 0x10000000;
		case 30 : return 0x20000000;
		case 31 : return 0x40000000;
		case 32 : return 0x80000000;

		case 0 :
		default : return 0;
	}

	return 0;
}

void stuaa_shiftr (int * self, int value) {
	int signedDebug = 0;

	if (stuaa_bitflag(BBIA_INTEGER_SIZE) & *self) {
		signedDebug = 1;
		*self &= ~stuaa_bitflag(BBIA_INTEGER_SIZE);
	}

	*self >>= value;

	if (signedDebug) {
		signedDebug = 0;
		*self |= stuaa_bitflag(BBIA_INTEGER_SIZE-value);
	}
}

void stuaa_sign_change (int * toChange) {

	if (!toChange || *toChange == 0) return;

	if (*toChange < 0)
		*toChange = ~*toChange + 1;
	else if (*toChange > 0)
		*toChange = ~(*toChange - 1);
}

static int outofbounders_max_bitDecay (int to, int test, int bitDec) {

	if (bitDec < 1) return 0;

	if (to & stuaa_bitflag (bitDec) && test & stuaa_bitflag (bitDec)) return 1;

	else if (to & stuaa_bitflag (bitDec) || test & stuaa_bitflag (bitDec)) {
		if (to & stuaa_bitflag (bitDec-1) && test & stuaa_bitflag (bitDec-1)) return 1;
	}

	else if (to & stuaa_bitflag (bitDec) || test & stuaa_bitflag (bitDec))
		return outofbounders_max_bitDecay (to, test, bitDec - 2);

	return 0;
}

int stuaa_outofbounders_max (int to, int test) {

	return outofbounders_max_bitDecay (to,test, BBIA_INTEGER_SIZE);
}

int stuaa_outofbounders_min (int to, int test) {
	int curToBit = 0, curTestBit = 0;

	for (int curBit = BBIA_INTEGER_SIZE; curBit > 0; curBit--) {
		curToBit = to & stuaa_bitflag (curBit);
		curTestBit = test & stuaa_bitflag (curBit);

		if ( curToBit != curTestBit ) {
			if (curTestBit != 0) return 1;
			return 0;
		}
	}
	return 0;
}

int stuaa_compare (int a, int b) {
	if (a > b) {
		if (stuaa_bitflag(BBIA_INTEGER_SIZE) & b) return -1;
		else return 1;
	}
	else if (a < b) {
		if (stuaa_bitflag(BBIA_INTEGER_SIZE) & a) return 1;
		else return -1;
	}
	return 0;
}

int stuaa_isPowerOfTwo (int value) {

	for (int curBit = 2; curBit <= BBIA_INTEGER_SIZE; curBit++)
		if (value == stuaa_bitflag (curBit))
			return curBit - 1;

	return -2;
}

char * stuaa_toBase (int sinteger, int base) {

	if ( !(base < 65 && base > 1) ) {
		throw ("The base must be from 2 to 64");
		return NULL;
	}

	if (base == 2) {

		char * result = calloc (sizeof(char), BBIA_INTEGER_SIZE + 1);
		if (result == NULL) abort();

		for (
			int currentBit = 1;

			currentBit <= BBIA_INTEGER_SIZE;

			result[BBIA_INTEGER_SIZE-currentBit] =
			(sinteger & stuaa_bitflag(currentBit)) ? '1' : '0',
			currentBit++
		);

		return result;
	}

	int powerOfTwo = stuaa_isPowerOfTwo (base);
	if (powerOfTwo != -2) {

		int start = ceil ( log_base (base, (unsigned)sinteger) ) - 1;

		char * result = calloc (sizeof(char), start + 2);
		if (result == NULL) abort();

		int tempValue = 0;
		for (int curBit = 0; curBit < BBIA_INTEGER_SIZE; curBit += powerOfTwo) {
			for (int curBitInTwo = 1; curBitInTwo <= powerOfTwo; curBitInTwo++) {
				tempValue |=
				(stuaa_bitflag(curBit + curBitInTwo) & sinteger)
				? stuaa_bitflag (curBitInTwo) : 0;
			}

			result[start--] = numerics[tempValue];
			tempValue = 0;
		}

		return result;
	}

	return stuaa_toBase_Clang (sinteger, base);
}

int stuaa_fromBase (char * integer, int base) {

	if (integer == NULL) {
		throw ("null pointer in stuaa_fromBase()");
		return -1;
	}

	if (base == 2) {

		int result = 0;

		for (
			int currentBit = 1;

			currentBit <= BBIA_INTEGER_SIZE;

			result |= (integer[BBIA_INTEGER_SIZE-currentBit] == '1')
			? stuaa_bitflag(currentBit) : 0,
			currentBit++
		);


		return result;
	}

	int powerOfTwo = stuaa_isPowerOfTwo (base);
	if (powerOfTwo != -2) {

		int result = 0;
		int size = strlen(integer);
		int tempValue = 0;

		for (int curChar = size-1, curMult = 0; curChar >= 0; curChar--, curMult++) {
			tempValue = strchr (numerics, integer[curChar]) - numerics;

			for (int curBit = 1; curBit <= powerOfTwo; curBit++) {
				result |= (tempValue & stuaa_bitflag(curBit)) ?
				stuaa_bitflag (curBit + powerOfTwo * curMult) : 0;
			}
		}

		return result;
	}

	return stuaa_fromBase_Clang (integer, base);
}

char * stuaa_toBase_Clang (unsigned integer, int base) {

	if ( !(base < 65 && base > 1) ) {
		throw ("The base must be from 2 to 64");
		return NULL;
	}

	char * result = calloc (sizeof(char), BBIA_INTEGER_SIZE + 1);
	if (result == NULL) abort();

	for (
		int start = ceil ( log_base (base, integer) ) - 1;

		integer != 0;

		result[start] = numerics [ integer % base ],
		integer /= base,
		start--
	);

	return result;
}

int stuaa_fromBase_Clang (char * integer, int base) {

	if (integer == NULL) {
		throw ("null pointer in stuaa_fromBase()");
		return -1;
	}

	unsigned int result = 0;

	for (
		int curDigit = 0;

		curDigit < BBIA_INTEGER_SIZE;

		result *= base,
		result += strchr (numerics, integer[curDigit]) - numerics,
		curDigit++
	);

	return (signed) result;
}
