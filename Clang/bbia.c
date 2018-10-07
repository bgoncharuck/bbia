#include "stuaa.h"
#include "bbia.h"

#include <stdio.h>
#include <stdlib.h>

#define throw(MSG) fprintf(stderr, "%s\n",MSG)

struct __bbia {

	// some languages don`t have unsigned int
	signed int at[BBIA_LEVEL_COUNT];
	signed int sign;
};

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
