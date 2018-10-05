#include "bbia.h"

#include <stdio.h>
#include <stdlib.h>

#define throw(MSG) fprintf(stderr, "%s\n",MSG)

struct __bbia {

	// some languages don`t have unsigned int
	signed int at[BBIA_LEVEL_COUNT];
	signed int sign;
};

void bbia_value_set_fromLevel (bbia * self, int level, int value) {

	if (self == NULL) {
		throw("null pointer in bbia_value_set_fromLevel()");
		return;
	}

	for (int i = level; i >= 0; i--)
		self->at[i] = value;
}

void bbia_value_set_toLevel (bbia * self, int level, int value) {

	if (self == NULL) {
		throw("null pointer in bbia_value_set_fromLevel()");
		return;
	}

	for (int i = level; i <= BBIA_LEVEL_TOP; i++)
		self->at[i] = value;
}

void bbia_value_set (bbia * self, int value) {

	bbia_value_set_fromLevel (self, BBIA_LEVEL_TOP, value);
}

int bbia_at_get (bbia * self, int index) {

	if (self == NULL) {
		throw("null pointer in bbia_at_get()");
		return -42;
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

	for (int i = 0; i <= BBIA_LEVEL_TOP; i++) {

		printf("%d_", self->at[i]);
	}
	puts("");
}

bbia * bbia_new (void) {

	bbia * self = malloc (sizeof(bbia));
	if (self == NULL) abort();

	self->sign = BBIA_UNSIGNED;
	bbia_value_set (self, BBIA_LEVEL_IS_EMPTY);

	return self;
}

void bbia_free (bbia * self) {

	if (self == NULL) {
		throw("null pointer in bbia_free()");
		return;
	}

	free(self);
}
