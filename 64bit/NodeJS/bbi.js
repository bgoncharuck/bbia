const uint = require('./ieee754_asInt.js');
const constants = require('./bbi_constants');

function bbi () {
	this.at = new Array(constants.LEVEL_COUNT).fill(0);
}
