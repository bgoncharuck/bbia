const uint = require('./ieee754_asInt.js');
const constants = require('./bbi_constants');



class bbi {
	constructor () {
		if (arguments.length == 2 && arguments[0] instanceof Boolean && arguments[1] instanceof Number) {
			this.at = new Array(constants.LEVEL_COUNT).fill(0);
			this.at[constants.LEVEL_TOP] = arguments[1];
			this.sign = arguments[0];
			this.lvlButton = constants.LEVEL_TOP;
		}
		else if (arguments.length && arguments[0] instanceof bbi) {
			this.at = arguments[0].at.slice(0);
			this.sign = arguments[0].sign;
			this.lvlButton = arguments[0].lvlButton;
		}
		else {
			this.at = new Array(constants.LEVEL_COUNT).fill(0);
			this.sign = false;
			this.lvlButton = constants.LEVEL_TOP;
		}
	}

}

let bbiConstructorTest_1 = new bbi();
let bbiConstructorTest_2 = new bbi(false, 7);
let bbiConstructorTest_3 = new bbi(bbiConstructorTest_2);
console.log(bbiConstructorTest_1 instanceof bbi);
console.log(bbiConstructorTest_2 instanceof bbi);
console.log(bbiConstructorTest_3 instanceof bbi);

module.exports = {

};
