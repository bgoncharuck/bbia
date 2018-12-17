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

module.exports = {
	bbi
}
