const uint = require('./ieee754_asInt.js');
const constants = require('./bbi_constants');

class bbi {
	constructor () {
		// from system integer
		if (arguments.length == 2 && arguments[0] instanceof Boolean && arguments[1] instanceof Number) {
			this.at = new Array(constants.LEVEL_COUNT).fill(0);
			this.at[constants.LEVEL_TOP] = arguments[1];
			this.sign = arguments[0];
			this.lvlButton = constants.LEVEL_TOP;
		}
		// clone
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

	Sign_Change () { this.sign = (this.sign) ? false : true; }
	Sign_Set (newSign) {
		if (newSign instanceof Boolean)
			this.sign = newSign;
	}

	LvlButton_Configure () {
		for (let curLvl = 0; curLvl < constants.LEVEL_TOP; curLvl++)
			if (this.at[curLvl] !== constants.LEVEL_IS_EMPTY) {
				this.lvlButton = curLvl;
				return;
			}
			this.lvlButton = constants.LEVEL_TOP;
	}

	LvlButton_Set (level) {
		if (level instanceof Number)
			this.lvlButton = level;
	}

	
}

module.exports = {
	bbi
}
