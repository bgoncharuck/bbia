#pragma once

#include <limits.h>

#define BBIA_INTEGER_SIZE 32

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
#define BBIA_BITS_2097152 2097152/BBIA_INTEGER_SIZE // 2^21
#define BBIA_BITS_4194304 4194304/BBIA_INTEGER_SIZE // 2^22
#define BBIA_BITS_8388608 8388608/BBIA_INTEGER_SIZE // 2^23
#define BBIA_BITS_16777216 16777216/BBIA_INTEGER_SIZE // 2^24
#define BBIA_BITS_33554432 33554432/BBIA_INTEGER_SIZE // 2^25
#define BBIA_BITS_67108864 67108864/BBIA_INTEGER_SIZE // 2^26
#define BBIA_BITS_134217728 134217728/BBIA_INTEGER_SIZE // 2^27
#define BBIA_BITS_268435456 268435456/BBIA_INTEGER_SIZE // 2^28
#define BBIA_BITS_536870912 536870912/BBIA_INTEGER_SIZE // 2^29
#define BBIA_BITS_1073741824 1073741824/BBIA_INTEGER_SIZE // 2^30


/*LEVEL*/
/*@CHOOSE size here*/
#define BBIA_LEVEL_COUNT BBIA_BITS_512

#define BBIA_BITS_COUNT BBIA_LEVEL_COUNT * BBIA_INTEGER_SIZE
#define BBIA_LEVEL_TOP -1 + BBIA_LEVEL_COUNT

/*DIGIT*/
// signed int 1111...n (n == BBIA_INTEGER_SIZE)
#define BBIA_LEVEL_IS_FULL -1
#define BBIA_LEVEL_IS_PFULL INT_MAX
#define BBIA_LEVEL_IS_NFULL INT_MIN
// signed int 0000...n (n == BBIA_INTEGER_SIZE)
#define BBIA_LEVEL_IS_EMPTY 0

/*SIGN*/
#define BBIA_SIGNED 1
#define BBIA_UNSIGNED 0

typedef struct __bbia bbia;

// @CONSTRUCTOR
bbia * bbia_new (void) ;
void bbia_free (bbia * self) ;
void bbia_check_usedLevelPosition (bbia * self) ;
// @BIT_OPERATIONS
void bbia_bits_shift_left (bbia * self, int value) ;
void bbia_bits_shift_right (bbia * self, int value) ;
void bbia_bits_or (bbia * first, bbia * second) ;
bbia * bbia_bits_or_new (bbia * first, bbia * second) ;
void bbia_bits_and (bbia * first, bbia * second) ;
bbia * bbia_bits_and_new (bbia * first, bbia * second) ;
void bbia_bits_flag_set (bbia * self, int num) ;
void bbia_bits_flag_unset (bbia * self, int num) ;
void bbia_bits_flag_set_mult (bbia * self, int * numArray) ;
void bbia_bits_flag_unset_mult (bbia * self, int * numArray) ;
bbia * bbia_bits_flag (int num) ;
bbia * bbia_bits_isFull () ;
bbia * bbia_bits_isEmpty () ;
bbia * bbia_bits_tillBit_isFull (int num) ;
bbia * bbia_bits_tillBit_isEmpty (int num) ;
