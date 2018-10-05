#pragma once

#define BITS_256 16
#define BITS_512 32

/*@CHOOSE size here*/
#define LEVEL_SIZE BITS_512

#define LEVEL_TOP -1 + LEVEL_SIZE

typedef struct __bbia bbia;

void bbia_set_value_fromLevel (bbia * self, int level, int value) ;

void bbia_set_value_toLevel (bbia * self, int level, int value) ;

void bbia_set_value (bbia * self, int value) ;

int bbia_at_get (bbia * self, int index) ;

void bbia_at_set (bbia * self, int index, int value) ;

bbia * bbia_new (void) ;

void bbia_free (bbia * self) ;

void bbia_print_levelValue (bbia * self) ;
