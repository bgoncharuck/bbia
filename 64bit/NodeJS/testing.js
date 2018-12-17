const uint = require('./ieee754_asInt.js');
const constants = require('./bbi_constants.js');
const bbi = require('./bbi.js');

// let uintToBaseCheck = 0xFFFFFFFF;
// let uintToBaseCheck = constants.LEVEL_IS_FULL;
let uintToBaseCheck = 7;
console.log(`${uint.toBaseOfTwo(uintToBaseCheck,2)}`);
console.log(`${uint.toBaseOfTwo(uintToBaseCheck,16)}`);
