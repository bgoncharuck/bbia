// Because of ieee754_asInt in development for now, it`s mostly dummy or STUAA
// const INTEGER_SIZE = 64;

// 32(52) bit from 64 can be used in ieee754 for integer, for full using of 64 bits needs ieee754_asInt
const INTEGER_SIZE = 32;
// @Warning: but it will use memory like 64 bit dtype

module.exports = {
	bitflag: (bit) => {

	},

	bitenabled: (num, bit) => {

	},

	bitand: (a, b) => {

	},

	bitor: (a, b) => {

	},

	bitnot (a, b) => {

	},

	bitnotand (a, b) => {

	},

	bitnotor (a, b) => {

	},

	bitxor (a, b) => {

	},

	bitshift_right: (num, size) => {

	},

	bitshift_left: (num, size) => {

	},

	bigger: (what, then) => {

	},

	lesser: (what, then) => {

	},

	equal: (what, to) => {

	},

	biggerequal: (what, then) => {

	},

	lesserequal: (what, then) => {

	},

	compare: (a, b) => {

	},

	outofmax: (to, test) => {

	},

	outofmin: (to, test) => {

	},

	isBaseOfTwo (num) => {

	},

	toBaseOfTwo (num, base) => {

	},

	fromBaseOfTwo (num, base) => {

	}
};
