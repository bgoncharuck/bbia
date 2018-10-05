#pragma once

#include <limits.h>

/*SIZE*/
#define BBIA_BITS_512 16 // 2^9
#define BBIA_BITS_1024 32 // 2^10
#define BBIA_BITS_2048 64 // 2^11
#define BBIA_BITS_4096 128 // 2^12
#define BBIA_BITS_8192 256 // 2^13
#define BBIA_BITS_16384 512 // 2^14
#define BBIA_BITS_32768 1024 // 2^15
#define BBIA_BITS_65536 2048 // 2^16
#define BBIA_BITS_131072 4096 // 2^17
#define BBIA_BITS_262144 8192 // 2^18
#define BBIA_BITS_524288 16384 // 2^19
#define BBIA_BITS_1048576 32768 // 2^20

/*LEVEL*/
/*@CHOOSE size here*/
#define BBIA_LEVEL_COUNT BBIA_BITS_512
#define BBIA_LEVEL_TOP -1 + BBIA_LEVEL_COUNT

/*DIGIT*/
#define BBIA_LEVEL_IS_FULL INT_MAX
#define BBIA_LEVEL_IS_EMPTY 0

/*SIGN*/
#define BBIA_SIGNED 1
#define BBIA_UNSIGNED 0

typedef struct __bbia bbia;

void bbia_set_value_fromLevel (bbia * self, int level, int value) ;

void bbia_set_value_toLevel (bbia * self, int level, int value) ;

void bbia_set_value (bbia * self, int value) ;

int bbia_at_get (bbia * self, int index) ;

void bbia_at_set (bbia * self, int index, int value) ;

bbia * bbia_new (void) ;

void bbia_free (bbia * self) ;

void bbia_print_levelValue (bbia * self) ;
