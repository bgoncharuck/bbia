const uint = require('./ieee754_asInt.js');
const Constants = require('./bbi_constants');

function typeis (what, is) {
	if (is === "bbi")
		return what instanceof bbi;
	return typeof what === is;
}

class bbi {
	constructor () {
		// from system integer
		if (arguments.length === 2 && typeis (arguments[0], "boolean") && typeis (arguments[1], "number") ) {
			this.at = new Array(Constants.LEVEL_COUNT).fill(0);
			this.at[Constants.LEVEL_TOP] = arguments[1];
			this.sign = arguments[0];
			this.lvlButton = Constants.LEVEL_TOP;
		}
		// clone
		else if (arguments.length === 1 && typeis (arguments[0], "bbi") ) {
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
		if ( typeis (newSign, "boolean") )
			this.sign = newSign;
	}

	LvlButton_Configure () {
		for (let curLvl = 0; uint.lesser (curLvl, Constants.LEVEL_TOP); curLvl++)
			if (uint.equal (this.at[curLvl], Constants.LEVEL_IS_EMPTY) === false) {
				this.lvlButton = curLvl;
				return;
			}
			this.lvlButton = Constants.LEVEL_TOP;
	}

	LvlButton_Set (level) {
		if ( typeis (level, "number") )
			this.lvlButton = level;
	}

	_setters_op (val, sign) {
		if (val != Constants.LEVEL_IS_EMPTY) {
			for (let curLvl = 0; uint.lesserequal (curLvl, Constants.LEVEL_TOP); curLvl++)
				this.at[curLvl] = val;
			this.sign = sign;
			this.lvlButton = 0;
		}
		else {
			for (let curLvl = 0; uint.lesserequal (curLvl, Constants.LEVEL_TOP); curLvl++)
				this.at[curLvl] = Constants.LEVEL_IS_EMPTY;
			this.sign = false;
			this.lvlButton = Constants.LEVEL_TOP;
		}
	}

	Set_Zero () { this._setters_op (Constants.LEVEL_IS_EMPTY, false); }
	Set_Min () { this._setters_op (Constants.LEVEL_IS_FULL, true); }
	Set_Max () { this._setters_op (Constants.LEVEL_IS_FULL, false); }
	Set_Value (value) {
		if (typeis (value, "number") )
			this._setters_op (value, false);
	}
	Set_ValueSigned (value) {
		if (typeis (value, "number") )
			this._setters_op (value, true);
	}

	Set_ValueFromLevel (level, value) {
		if (typeof level === typeof value && typeis (value, "number") ) {
			for (let curLvl = 0; uint.lesserequal (curLvl, level); curLvl++)
			this.at[curLvl] = value;
			this.sign = false;
			this.lvlButton = 0; if (uint.equal (value, Constants.LEVEL_IS_EMPTY)) this.LvlButton_Configure();
		}
	}

	Set_ValueFromLevelSigned (level, value) {
		this.Set_ValueFromLevel (level,value);
		this.sign = true;
	}

	Set_ValueToLevel (level, value) {
		if (typeof level === typeof value && typeis (value, "number") ) {
			for (let curLvl = level; lesserequal (curLvl, Constants.LEVEL_TOP); curLvl++)
				this.at[curLvl] = value;
			this.sign = false;
			this.lvlButton = level; if (uint.equal (value, Constants.LEVEL_IS_EMPTY)) this.LvlButton_Configure();
		}
	}

	Set_ValueToLevelSigned (level, value) {
		this.Set_ValueToLevel (level,value);
		this.sign = true;
	}

	Set_SystemInteger (isSigned, integer) {
		if (typeis (isSigned, "boolean") && typeis (integer, "number")) {
			this.Set_Zero();
			this.at[Constants.LEVEL_TOP] = integer;
			this.sign = isSigned;
		}
	}


	Check_IsInteger (integer) {
		if (typeis (integer, "number") === false) return false;
		else if (this.lvlButton < Constants.LEVEL_TOP) return false;
		return (uint.equal (this.at[Constants.LEVEL_TOP], integer)) ? true : false;
	}

	Check_IsZero () { return this.Check_IsInteger (0); }
	Check_IsOne () { return this.Check_IsInteger (1); }
	Check_IsSystemInteger () { return (uint.equal (this.lvlButton, Constants.LEVEL_TOP)) ? true : false; }

	CompareUnsigned (toCompare) {
		if (typeis (toCompare, "bbi")) {
			let curCompare = 0;
			let curLvl = (uint.lesserequal (this.lvlButton, toCompare.lvlButton)) ? this.lvlButton : toCompare.lvlButton;
			for (; uint.lesserequal (curLvl, Constants.LEVEL_TOP); curLvl++) {
				curCompare = uint.compare (this.at[curLvl], toCompare.at[curLvl]);
				if (curCompare !== 0)
				return curCompare;
			}
			return 0;
		}
		else if (typeis (toCompare, "number")) {
			if (uint.lesser (this.lvlButton, Constants.LEVEL_TOP)) return 1;
			return uint.compare (this.at[Constants.LEVEL_TOP], toCompare);
		}
	}

	Compare () {
		if (arguments.length === 2 && typeis (arguments[0], "boolean") && typeis (arguments[1], "number")) {
			let res = this.CompareUnsigned (arguments[1]);
			if (this.sign == arguments[0]) return res;
			else if (this.sign == true) return -1;
			else if (arguments[0] == true) return 1;
		}
		else if (arguments.length === 1 && typeis (arguments[0], "bbi")) {
			let res = this.CompareUnsigned (arguments[0]);
			if (this.sign == arguments[0].sign) return res;
			else if (this.sign == true) return -1;
			else if (arguments[0].sign == true) return 1;
		}
		return 0;
	}

	Copy (from) {
		if (typeis (from, "bbi")) {
			this.at = from.at.slice(0);
			this.sign = from.sign;
			this.lvlButton = from.lvlButton;
		}
	}

	// @BIT_OPERATIONS

	Bits_ShiftLeft (value) {
		if (typeis (value, "number") && uint.lesserequal (value, Constants.INTEGER_SIZE)) {

			let savedBits = new Array(Constants.LEVEL_TOP).fill(0);
			let bitMask = [0,0];
			for (let curBit = Constants.INTEGER_SIZE-value+1; curBit <= Constants.INTEGER_SIZE; curBit++)
				bitMask[0] = uint.bitor (bitMask[0], uint.bitflag(curBit));
			// 1. Shift with saved bits
			// we shift all levels from top to zero
			// but save the part which is lost
			// zero level not count in saving
			// we saved bits in position INTEGER_SIZE...INTEGER_SIZE-value
			// but need set bits in position value...1

			// 2. Set saved bits
			// for all levels lesser then top
			// we set saved bits
			this.lvlButton = (this.lvlButton == 0) ? 0 : this.lvlButton - 1;
			for (let lvl = Constants.LEVEL_TOP; uint.bigger (lvl, this.lvlButton); this.at[lvl] = uint.bitshift_left (this.at[lvl], value), lvl--) {
				bitMask[1] = uint.bitand (this.at[lvl], bitMask[0]);
				bitMask[1] = uint.bitshift_right (bitMask[1], Constants.INTEGER_SIZE-value);
				savedBits[lvl-1] = bitMask[1];
			}
			this.at[this.lvlButton] = uint.bitshift_left (this.at[this.lvlButton], value);

			for (let lvl = this.lvlButton; uint.lesser (lvl < Constants.LEVEL_TOP); lvl++)
				this.at[lvl] = uint.bitor (this.at[lvl], savedBits[lvl]);
		}
		else if (uint.biggerequal (value, Constants.INTEGER_SIZE))
			this.Set_Zero();
	}

	Bits_ShiftRight (value) {
		if (typeis (value, "number") && uint.lesserequal (value, Constants.INTEGER_SIZE)) {

			let savedBits = new Array(Constants.LEVEL_TOP).fill(0);
			let bitMask = [0,0];
			for (let curBit = 1; curBit <= value; curBit++)
				bitMask[0] = uint.bitor (bitMask[0], uint.bitflag(curBit));
			// 1. Shift with saved bits
			// we shift all levels from zero to top
			// but save the part which is lost
			// we saved bits in position 1...value
			// but need set bits in position INTEGER_SIZE-value...INTEGER_SIZE
			// top level not count in saving

			// 2. Set saved bits
			// for all levels bigger then 0
			// we set saved bits
			for (let lvl = this.lvlButton; uint.lesser (lvl, Constants.LEVEL_TOP); this.at[lvl] = uint.bitshift_right (this.at[lvl], value), lvl++) {
				bitMask[1] = uint.bitand (this.at[lvl], bitMask[0]);
				bitMask[1] = uint.bitshift_left (bitMask[1], Constants.INTEGER_SIZE-value);
				savedBits[lvl] = bitMask[1];
			}
			this.at[Constants.LEVEL_TOP] = uint.bitshift_right (this.at[Constants.LEVEL_TOP], value);

			for (let lvl = this.lvlButton; uint.lesser (lvl, Constants.LEVEL_TOP); lvl++)
				this.at[lvl+1] = uint.bitor (this.at[lvl+1], savedBits[lvl]);
			if (uint.equal (this.at[this.lvlButton], Constants.LEVEL_IS_EMPTY)) this.lvlButton++;
		}
		else if (uint.biggerequal (value, Constants.INTEGER_SIZE))
			this.Set_Zero();
	}

}

module.exports = {
	bbi
}
