# Bit Big Integer Algorithm v. 0.1.3

BBIA is superstructure on system integer and a try to create fast and language independent algorithm which can save Nbit integer.

# Mathematical Interpretation
The max value that can be saved with this algorithm is two, in power of two in power of max integer value, decreased by one (2^(2^N-1))-1

<p>For 32bit integer max value is (2^(2^32-1))-1 and need 15.(9) == 16 GigaByte of RAM (2^32-1 * 4 byte) to save it
<p>For 64bit integer max value is (2^(2^64-1))-1 and need 127.(9) == 128 ExaByte of RAM (2^64-1 * 8 byte) to save it
<p>Don't forget to count additional integer for sign and used level save. Also need extra memory for operations. The bigger count of levels the more memory you need for this operations.

Also BBIA can be considered as canonical and shifted numeric system of integer values with additional sign save
[pure, positional, with direct presentation of forms and natural weight order]

Decimal Binary Code == "8,4,2,1" (direct)
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
* **bbia and integer multiplication**
* **bbia and integer division**
* bbia and bbia sum, dif, multiplication, division
* from and to base of 2 in string
* sqrt, log (base, bbia)

## Current tested bit sizes for 32bit integer size
<p> 2^30 bits and 4byte integer
<p> 2^(2^30)-1 is max value
<p> needs 4 GigaByte of RAM to store
<p> computer with 8 GigaBytes of RAM can easily store it
<p> computer with 32 GigaBytes of RAM is comfortable to use arithmetic operations with it
<p>
<p> 2^29 bits and 4byte integer
<p> 2^(2^29)-1 is max value
<p> needs 2 GigaByte of RAM to store
<p> computer with 4 GigaBytes of RAM can easily store it
<p> computer with 16 GigaBytes of RAM is comfortable to use arithmetic operations with it
