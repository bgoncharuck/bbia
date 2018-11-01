# Bit Big Integer Algorithm v. 0.0.2

BBIA is a try to create fast and language independent algorithm which can save Nbit integer.

The max value that can be saved with this algorithm is maximum unsigned integer value in power of two.
For 32bit integer max value is 2^32*2^32 and need 2^11 PetaByte of RAM;
For 64bit integer max value is 2^64*2^64 and need 2^35 YottaByte of RAM;
Don't forget to count additional integer for sign save.

Also BBIA can be considered as canonical and shifted numeric system of integer values with additional sign save
[pure, positional, with direct presentation of forms and natural weight order]

Decimal Binary Code == "8,4,2,1"
min == 0000..0 [Nbit] max == 1111..1 [Nbit] => min == 0 max == -1
(for any signed integer datatype used as level)

## Releases
First release will be in 24 january of 2019

## Syntax
Algorithm will be written in C language syntax.
Some languages don't have unsigned integer type, so was created
[Signed To Unsigned and Arithmetic Algorithm for BBIA](https://github.com/bgoncharuck/stuaa)

## Languages
**Before release, any working code can be found in Algorithm subfolder.**
If you try to use other realizations (languages) that will not work.

It will support any statically typed language.
I will add simple port to C (unsigned ver), Java, C# and JavaScript(TypeScript) for every release.
Will be added Rust version. You are free to add other languages.

## What works for now?
* STUAA (bitflag, shiftr, outofbounders)
* bitflag, multi bitflag
* bits fulll/empty till
* **bitshift right, bitshift left**
* bbia and integer sum
* bbia and integer dif
* **bbia and integer mult**
* bbia and bbia sum, mult
