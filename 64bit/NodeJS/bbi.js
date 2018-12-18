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
		for (let curLvl = 0; curLvl < Constants.LEVEL_TOP; curLvl++)
			if (this.at[curLvl] !== Constants.LEVEL_IS_EMPTY) {
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
			for (let curLvl = 0; curLvl <= level; curLvl++)
			this.at[curLvl] = value;
			this.sign = false;
			this.lvlButton = 0; if (value === Constants.LEVEL_IS_EMPTY) this.LvlButton_Configure();
		}
	}

	Set_ValueFromLevelSigned (level, value) {
		this.Set_ValueFromLevel (level,value);
		this.sign = true;
	}

	Set_ValueToLevel (level, value) {
		if (typeof level === typeof value && typeis (value, "number") ) {
			for (let curLvl = level; curLvl <= Constants.LEVEL_TOP; curLvl++)
				this.at[curLvl] = value;
			this.sign = false;
			this.lvlButton = level; if (value === Constants.LEVEL_IS_EMPTY) this.LvlButton_Configure();
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
		return (this.at[Constants.LEVEL_TOP] == integer) ? true : false;
	}

	Check_IsZero () { return this.Check_IsInteger (0); }
	Check_IsOne () { return this.Check_IsInteger (1); }
	Check_IsSystemInteger () { return (this.lvlButton == Constants.LEVEL_TOP) ? true : false; }

	CompareUnsigned (toCompare) {
		if (typeis (toCompare, "bbi")) {
			let curCompare = 0;
			let curLvl = (this.lvlButton <= toCompare.lvlButton) ? this.lvlButton : toCompare.lvlButton;
			for (; curLvl <= Constants.LEVEL_TOP; curLvl++) {
				curCompare = uint.compare (this.at[curLvl], toCompare.at[curLvl]);
				if (curCompare !== 0)
				return curCompare;
			}
			return 0;
		}
		else if (typeis (toCompare, "number")) {
			if (this.lvlButton < Constants.LEVEL_TOP) return 1;
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
}

module.exports = {
	bbi
}
