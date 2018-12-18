const uint = require('./ieee754_asInt.js');
const Constants = require('./bbi_constants.js');
const {bbi} = require('./bbi.js');

// let uintToBaseCheck = 0xFFFFFFFF;
// let uintToBaseCheck = Constants.LEVEL_IS_FULL;
let uintToBaseCheck = 7;
console.log(`${uint.toBaseOfTwo(uintToBaseCheck,2)}`);
console.log(`${uint.toBaseOfTwo(uintToBaseCheck,16)}`);

let bbiConstructorTest_1 = new bbi();
let bbiConstructorTest_2 = new bbi(false, 7);
let bbiConstructorTest_3 = new bbi(bbiConstructorTest_2);
console.log(bbiConstructorTest_1 instanceof bbi);
console.log(bbiConstructorTest_2 instanceof bbi);
console.log(bbiConstructorTest_3 instanceof bbi);
console.log(bbiConstructorTest_3);
bbiConstructorTest_3.at[Constants.LEVEL_TOP] = 12;
let bbiBitOrNewTest = bbiConstructorTest_2.Bits_Or_New(bbiConstructorTest_3);
console.log (bbiBitOrNewTest);

/*
function kek (a,b) {a |= b};
let a = 0;
let b = uint.bitflag(3);
kek (a,b);
console.log(`${uint.toBaseOfTwo(a,2)}`);
bbiConstructorTest_2.Sign_Change();
console.log(bbiConstructorTest_2.sign == false);
bbiConstructorTest_2.Sign_Change();
console.log(bbiConstructorTest_2.sign == false);
*/

let test1 = new bbi(false, 3);
let test2 = new bbi(false, 7);
let test3 = new bbi(false, 2);
let test4 = new bbi(false, 32);
let test5 = new bbi(true, 45);

console.log (test1.toBaseOfTwo (16));
console.log (test2.toBaseOfTwo (16));
console.log (test3.toBaseOfTwo (16));
console.log (test4.toBaseOfTwo (16));
console.log (test5.toBaseOfTwo (16));
