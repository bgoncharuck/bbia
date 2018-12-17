const uint = require('./ieee754_asInt.js');
const constants = require('./bbi_constants');

function bbi () {
	this.at = new Array(constants.LEVEL_COUNT).fill(0);
	this.sign = false;
	this.lvlButton = constants.LEVEL_TOP;
}

// from system integer
function bbi (sign, integer) {
	this.at = new Array(constants.LEVEL_COUNT).fill(0);
	this.at[constants.LEVEL_TOP] = integer;
	this.sign = sign;
	this.lvlButton = constants.LEVEL_TOP;
}

// copy
function bbi (from) {
	this.at = from.at.slice(0);
	this.sign = from.sign;
	this.lvlButton = from.lvlButton;
}
