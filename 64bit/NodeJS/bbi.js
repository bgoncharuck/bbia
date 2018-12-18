const uint = require('./ieee754_asInt.js');
const Constants = require('./bbi_constants');

function istype (what, is) {
	return typeof what === is;
}

class bbi {
	constructor () {
		// from system integer
		if (arguments.length === 2 && istype (arguments[0], "boolean") && istype (arguments[1], "number") ) {
			this.at = new Array(Constants.LEVEL_COUNT).fill(0);
			this.at[Constants.LEVEL_TOP] = arguments[1];
			this.sign = arguments[0];
			this.lvlButton = Constants.LEVEL_TOP;
		}
		// clone
		else if (arguments.length === 1 && istype (arguments[0], "bbi") ) {
			this.at = arguments[0].at.slice(0);
			this.sign = arguments[0].sign;
			this.lvlButton = arguments[0].lvlButton;
		}
		else {
			this.at = new Array(Constants.LEVEL_COUNT).fill(0);
			this.sign = false;
			this.lvlButton = Constants.LEVEL_TOP;
		}
	}

	Sign_Change () { this.sign = (this.sign) ? false : true; }
	Sign_Set (newSign) {
		if ( istype (newSign, "boolean") )
			this.sign = newSign;
	}

	LvlButton_Configure () {
		for (let curLvl = 0; curLvl < Constants.LEVEL_TOP; curLvl++)
			if (this.at[curLvl] !== Constants.LEVEL_IS_EMPTY) {
				this.lvlButton = curLvl;
				return;
			}
			this.lvlButton = Constants.LEVEL_TOP;
	}

	LvlButton_Set (level) {
		if ( istype (level, "number") )
			this.lvlButton = level;
	}

	_setters_op (val, sign) {
		if (val != Constants.LEVEL_IS_EMPTY) {
			for (let curLvl = 0; curLvl <= Constants.LEVEL_TOP; curLvl++)
				this.at[curLvl] = val;
			this.sign = sign;
			this.lvlButton = 0;
		}
		else {
			for (let curLvl = 0; curLvl <= Constants.LEVEL_TOP; curLvl++)
				this.at[curLvl] = Constants.LEVEL_IS_EMPTY;
			this.sign = false;
			this.lvlButton = Constants.LEVEL_TOP;
		}
	}

}

module.exports = {
	bbi
}
