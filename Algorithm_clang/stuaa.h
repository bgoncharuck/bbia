#pragma once

int stuaa_bitflag (int num) ;

void stuaa_shiftr (int * self, int value) ;

void stuaa_sign_change (int * toChange) ;

int stuaa_outofbounders_max (int to, int test) ;

int stuaa_outofbounders_min (int to, int test) ;

int stuaa_compare (int a, int b) ;

int stuaa_isPowerOfTwo (int value) ;

int stuaa_findDigitInNumerics (const char * numicsStr, char digit) ;

char * stuaa_toBase_Clang (unsigned integer, int base) ;

int stuaa_fromBase_Clang (char * integer, int base) ;

char * stuaa_toBase (int integer, int base) ;

int stuaa_fromBase (char * integer, int base) ;
