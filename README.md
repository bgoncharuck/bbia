# Bit Big Integer Algorithm v. 0.0.2

BBIA is a try to create fast and language independent algorithm which can save Nbit integer.

# Mathematical Interpretation
The max value that can be saved with this algorithm is two in power of two in power of number of bit of integer, decreased by one, decreased by one.  (2^(2 ^ N - 1 ) ) -1
<p>For 32bit integer max value is (2^(2^31))-1 and need 8 GigaByte of RAM (2^31 * 4 byte)
<p>For 64bit integer max value is (2^(2^63))-1 and need 64 ExaByte of RAM (2^63 * 8 byte)
<p>Don't forget to count additional integer for sign save.

Also BBIA can be considered as canonical and shifted numeric system of integer values with additional sign save
[pure, positional, with direct presentation of forms and natural weight order]

Decimal Binary Code == "8,4,2,1"
<p>min == 0000..0 [Nbit] max == 1111..1 [Nbit] => min == 0 max == -1
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

<p>It will support any statically typed language.
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
