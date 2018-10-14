#pragma once

#define BBIA_INTEGER_SIZE 32
#define BBIA_BIT_LAST 0x80000000

/*SIZE*/
#define BBIA_BITS_512 512/BBIA_INTEGER_SIZE // 2^9
#define BBIA_BITS_1024 1024/BBIA_INTEGER_SIZE // 2^10
#define BBIA_BITS_2048 2048/BBIA_INTEGER_SIZE // 2^11
#define BBIA_BITS_4096 4096/BBIA_INTEGER_SIZE // 2^12
#define BBIA_BITS_8192 8192/BBIA_INTEGER_SIZE // 2^13
#define BBIA_BITS_16384 16384/BBIA_INTEGER_SIZE // 2^14
#define BBIA_BITS_32768 32768/BBIA_INTEGER_SIZE // 2^15
#define BBIA_BITS_65536 65536/BBIA_INTEGER_SIZE // 2^16
#define BBIA_BITS_131072 131072/BBIA_INTEGER_SIZE // 2^17
#define BBIA_BITS_262144 262144/BBIA_INTEGER_SIZE // 2^18
#define BBIA_BITS_524288 524288/BBIA_INTEGER_SIZE // 2^19
#define BBIA_BITS_1048576 1048576/BBIA_INTEGER_SIZE // 2^20


/*LEVEL*/
/*@CHOOSE size here*/
#define BBIA_LEVEL_COUNT BBIA_BITS_512
#define BBIA_LEVEL_TOP -1 + BBIA_LEVEL_COUNT

/*DIGIT*/
// signed int 1111...n (n == BBIA_INTEGER_SIZE)
#define BBIA_LEVEL_IS_FULL -1
// signed int 0000...n (n == BBIA_INTEGER_SIZE)
#define BBIA_LEVEL_IS_EMPTY 0

/*SIGN*/
#define BBIA_SIGNED 1
#define BBIA_UNSIGNED 0

typedef struct __bbia bbia;

// bbia
void bbia_bitshift_left (bbia * self, int value) ;

void bbia_bitshift_right (bbia * self, int value) ;

void bbia_bitflag_set (bbia * self, unsigned num) ;

void bbia_bitflag_unset (bbia * self, unsigned num) ;

void bbia_bitflag_set_mult (bbia * self, unsigned * numArray) ;

void bbia_bitflag_unset_mult (bbia * self, unsigned * numArray) ;

bbia * bbia_bitflag (unsigned num) ;

void bbia_set_value_fromLevel (bbia * self, unsigned level, int value) ;

void bbia_set_value_toLevel (bbia * self, unsigned level, int value) ;

void bbia_set_value (bbia * self, int value) ;

int bbia_at_get (bbia * self, unsigned level) ;

void bbia_at_set (bbia * self, unsigned level, int value) ;

void bbia_print_levelValue (bbia * self) ;

bbia * bbia_new (void) ;

void bbia_free (bbia * self) ;
