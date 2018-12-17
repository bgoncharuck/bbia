// Because of ieee754_asInt in development for now, it`s mostly dummy or STUAA
// const INTEGER_SIZE = 64;

// 32(52) bit from 64 can be used in ieee754 for integer, for full using of 64 bits needs ieee754_asInt
const INTEGER_SIZE = 32;
// @Warning: but it will use memory like 64 bit dtype

function bitflag (bit) {
	if ( !(bit >= 0 && bit <= INTEGER_SIZE) )
		return 0;

	switch (bit) {
		case 1 : return 0x1;
		case 2 : return 0x2;
		case 3 : return 0x4;
		case 4 : return 0x8;
		case 5 : return 0x10;
		case 6 : return 0x20;
		case 7 : return 0x40;
		case 8 : return 0x80;
		case 9 : return 0x100;
		case 10 : return 0x200;
		case 11 : return 0x400;
		case 12 : return 0x800;
		case 13 : return 0x1000;
		case 14 : return 0x2000;
		case 15 : return 0x4000;
		case 16 : return 0x8000;
		case 17 : return 0x10000;
		case 18 : return 0x20000;
		case 19 : return 0x40000;
		case 20 : return 0x80000;
		case 21 : return 0x100000;
		case 22 : return 0x200000;
		case 23 : return 0x400000;
		case 24 : return 0x800000;
		case 25 : return 0x1000000;
		case 26 : return 0x2000000;
		case 27 : return 0x4000000;
		case 28 : return 0x8000000;
		case 29 : return 0x10000000;
		case 30 : return 0x20000000;
		case 31 : return 0x40000000;
		case 32 : return 0x80000000;

		case 0 :
		default : return 0;
	}
}

function shift_right (num, size) {
	let save = false;
	if (num & bitflag(INTEGER_SIZE)) {
		num &= ~(bitflag(INTEGER_SIZE));
		save = true;
	}

	num >>= size;
	if (save == true) num |= bitflag (INTEGER_SIZE-size);
}

function compare (a, b) {
	return -1;
}

function outofmax (to, test) {
	return false;
}

function outofmin (to, test) {
	return false;
}

function inBaseOfTwo (num) {
	return 2;
}

function toBaseOfTwo (num, base) {
	return "";
}

function fromBaseOfTwo (str, base) {
	return 0;
}

module.exports = {
	bitflag: (bit) => bitflag (bit),

	bitenabled: (num, bit) => num & bitflag (bit),

	bitand: (a, b) => a & b,

	bitor: (a, b) => a | b,

	bitnot: (num) => ~num,

	bitnotand: (a, b) => ~(a & b),

	bitnotor: (a, b) => ~(a | b),

	bitxor: (a, b) => a ^ b,

	bitshift_right: (num, size) => shift_right (num, size),

	bitshift_left: (num, size) => num <<= size,

	bigger: (what, then) => what > then,

	lesser: (what, then) => what < then,

	equal: (what, to) => what === to,

	biggerequal: (what, then) => what >= then,

	lesserequal: (what, then) => what <= then,

	compare: (a, b) => compare (a, b),

	outofmax: (to, test) => outofmax (to, test),

	outofmin: (to, test) => outofmin (to, test),

	add: (a, b) => a + b,

	sub: (from, what) => from - what,

	mult: (a, b) => a * b,

	div: (a, division) => a / division,

	mod: (a, division) => a % division,

	pow: (a, power) => a ** power,

	isBaseOfTwo: (num) => isBaseOfTwo (num),

	toBaseOfTwo: (num, base) => toBaseOfTwo (num, base),

	fromBaseOfTwo: (str, base) => fromBaseOfTwo (str, base)
};
