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
/*
bbiConstructorTest_2.Sign_Change();
console.log(bbiConstructorTest_2.sign == false);
bbiConstructorTest_2.Sign_Change();
console.log(bbiConstructorTest_2.sign == false);
*/
