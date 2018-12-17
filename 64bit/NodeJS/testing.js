const uint = require('./ieee754_asInt.js');

let curDigit = 0xFFFFFFFF;
console.log(`${uint.toBaseOfTwo(curDigit,2)}`);
console.log(`${uint.toBaseOfTwo(curDigit,16)}`);
