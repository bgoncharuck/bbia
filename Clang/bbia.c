#include <bbia.h>

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define throw(MSG) fprintf(stderr, "%s\n",MSG)

struct __bbia {

	// some languages don`t have unsigned int
	signed int at[LEVEL_SIZE];
	signed int sign;
};

void bbia_set_value_fromLevel (bbia * self, int level, int value) {

	if (self == NULL) {
		throw("null pointer in bbia_set_value_fromLevel()");
		return;
	}

	for (int i = level; i >= 0; i--)
		self->at[i] = value;
}

void bbia_set_value_toLevel (bbia * self, int level, int value) {

	if (self == NULL) {
		throw("null pointer in bbia_set_value_fromLevel()");
		return;
	}

	for (int i = level; i <= LEVEL_TOP; i++)
		self->at[i] = value;
}

void bbia_set_value (bbia * self, int value) {

	bbia_set_value_fromLevel (self, LEVEL_TOP, value);
}


bbia * bbia_new (void) {

	bbia * self = malloc (sizeof(bbia));
	if (self == NULL) abort();

	bbia->sign = 0;
	bbia_set_value (self, 0);

	return self;
}

void bbia_free (bbia * self) {

	if (self == NULL) {
		throw("null pointer in bbia_free()");
		return;
	}

	free(self);
}
