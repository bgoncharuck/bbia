const uint = require('./ieee754_asInt.js');
const Constants = require('./bbi_constants');

function typeis (what, is) {
	if (is === "bbi")
	return what instanceof bbi;
	else if (is === "array")
	return Array.isArray (what);
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
		if (val !== Constants.LEVEL_IS_EMPTY) {
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
			if (this.sign === arguments[0]) return res;
			else if (this.sign === true) return -1;
			else if (arguments[0] === true) return 1;
		}
		else if (arguments.length === 1 && typeis (arguments[0], "bbi")) {
			let res = this.CompareUnsigned (arguments[0]);
			if (this.sign === arguments[0].sign) return res;
			else if (this.sign === true) return -1;
			else if (arguments[0].sign === true) return 1;
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
			for (let lvl = Constants.LEVEL_TOP; uint.bigger (lvl, this.lvlButton); this.at[lvl] = uint.bitshift_left (this.at[lvl], value), lvl--) {
				bitMask[1] = uint.bitand (this.at[lvl], bitMask[0]);
				bitMask[1] = uint.bitshift_right (bitMask[1], Constants.INTEGER_SIZE-value);
				savedBits[lvl-1] = bitMask[1];
			}
			this.at[this.lvlButton] = uint.bitshift_left (this.at[this.lvlButton], value);

			for (let lvl = this.lvlButton; uint.lesser (lvl < Constants.LEVEL_TOP); lvl++)
				this.at[lvl] = uint.bitor (this.at[lvl], savedBits[lvl]);

			this.LvlButton_Configure();
		}
		else if (uint.biggerequal (value, Constants.INTEGER_SIZE))
			this.Set_Zero();
	}

	Bits_ShiftRight (value) {
		if (typeis (value, "number") && uint.lesserequal (value, Constants.INTEGER_SIZE)) {

			let savedBits = new Array(Constants.LEVEL_TOP).fill(0);
			let bitMask = [0,0];
			for (let curBit = 1; uint.lesserequal (curBit, value); curBit++)
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

	Bits_Or (second) {
		if (typeis (second, "bbi")) {

			this.lvlButton = (uint.lesserequal (this.lvlButton, second.lvlButton)) ? this.lvlButton : second.lvlButton;
			for (let curLvl = this.lvlButton; uint.lesserequal (curLvl, Constants.LEVEL_TOP); curLvl++)
			this.at[curLvl] = uint.bitor (this.at[curLvl], second.at[curLvl]);
		}
	}

	Bits_Or_New (second) {
		if (typeis (second, "bbi") === false) return new bbi();
		let result = new bbi(this);
		result.Bits_Or(second);
		return result;
	}

	Bits_And (second) {
		if (typeis (second, "bbi")) {

			this.lvlButton = (uint.lesserequal (this.lvlButton, second.lvlButton)) ? this.lvlButton : second.lvlButton;
			for (let curLvl = this.lvlButton; uint.lesserequal (curLvl, Constants.LEVEL_TOP); curLvl++)
			this.at[curLvl] = uint.bitand (this.at[curLvl], second.at[curLvl]);
		}
	}

	Bits_And_New (second) {
		if (typeis (second, "bbi") === false) return new bbi();
		let result = new bbi(this);
		result.Bits_And(second);
		return result;
	}

	Bits_FlagSet (bit) {
		if (typeis (bit, "number")) {

			let lvl = Constants.LEVEL_TOP - uint.div (bit, Constants.INTEGER_SIZE);
			bit %= Constants.INTEGER_SIZE;
			if (bit !== 0)
				this.at[lvl] = uint.bitor (this.at[lvl], uint.bitflag (bit));
			else
				this.at[lvl+1] |= uint.bitor (this.at[lvl], uint.bitflag (Constants.INTEGER_SIZE));
			this.LvlButton_Configure();
		}
	}

	Bits_FlagUnset (bit) {
		if (typeis (bit, "number")) {

			let lvl = Constants.LEVEL_TOP - uint.div (bit, Constants.INTEGER_SIZE);
			bit %= Constants.INTEGER_SIZE;
			if (bit !== 0)
				this.at[lvl] = uint.bitnotand (this.at[lvl], uint.bitflag (bit));
			else
				this.at[lvl+1] = uint.bitnotand (this.at[lvl+1], uint.bitflag (Constants.INTEGER_SIZE));
			this.LvlButton_Configure();
		}
	}

	Bits_FlagSetMult (bitFlagArray) {
		if (typeis (bitFlagArray, "array")) {

			for (let curFlag = 0; uint.lesser (curFlag, bitFlagArray.length); curFlag++)
				this.Bits_FlagSet (bitFlagArray[curFlag]);
		}
	}

	Bits_FlagUnsetMult (bitFlagArray) {
		if (typeis (bitFlagArray, "array")) {

			for (let curFlag = 0; uint.lesser (curFlag, bitFlagArray.length); curFlag++)
				this.Bits_FlagUnset (bitFlagArray[curFlag]);
		}
	}

		Bits_Flag (bit) {
			let result = new bbi();
			result.Bits_FlagSet (bit);
			return result;
		}

		Bits_Full () {
			let result = new bbi();
			for (let curLvl = 0; uint.lesserequal (curLvl, Constants.LEVEL_TOP); curLvl++)
				result.at[curLvl] = Constants.LEVEL_IS_FULL;
			result.lvlButton = 0;
			return result;
		}
		Bits_Empty () { return new bbi(); }

		Bits_FullTillBit (bit) {
			let result = new bbi();
			let lvl = Constants.LEVEL_TOP - uint.div (bit, Constants.INTEGER_SIZE);
			bit %= Constants.INTEGER_SIZE;
			if (bit === 0) lvl++;

			for (let curLvl = lvl + 1; uint.lesser (curLvl, Constants.LEVEL_TOP); curLvl--)
				result.at[curLvl] = Constants.LEVEL_IS_FULL;
			for (let curBit = 1; uint.lesser (curBit, bit); curBit++)
				result.at[lvl] = uint.bitor (result.at[lvl], uint.bitflag (curBit));
			this.LvlButton_Configure();
			return result;
		}

		Bits_EmptyTillBit (bit) {
			let result = new bbi();
			let lvl = Constants.LEVEL_TOP - uint.div (bit, Constants.INTEGER_SIZE);
			bit %= Constants.INTEGER_SIZE;
			if (bit === 0) lvl++;

			for (let curLvl = lvl + 1; uint.lesser (curLvl, Constants.LEVEL_TOP); curLvl--)
				result.at[curLvl] = Constants.LEVEL_IS_EMPTY;
			for (let curBit = 1; uint.lesser (curBit, bit); curBit++)
				result.at[lvl] = uint.bitnotand (result.at[lvl], uint.bitflag (curBit));
			this.LvlButton_Configure();
			return result;
		}

		// ADD/SUB private module operations

		_add_bbi_op (from) {
			// if from is system integer and out of bounders not possible then try simple language addition
			if (from.Check_IsSystemInteger() && uint.lesser (this.at[0], Constants.LEVEL_IS_FULL)) {
				this.Add_Level (from.at[Constants.LEVEL_TOP], Constants.LEVEL_TOP);
				return;
			}
			else {
				this.lvlButton = (uint.lesserequal (this.lvlButton, from.lvlButton) ) ? this.lvlButton : from.lvlButton;
				let level = Constants.LEVEL_TOP;
				let outBit = 0;
				let curBit = 0;

				// standart addition algorithm for binary N-bit integer
				for (; (uint.biggerequal (level, this.lvlButton)) || (outBit !== 0 && uint.bigger (level, 0)); level--)
				for (let bitPos = 1; uint.lesserequal (bitPos, Constants.INTEGER_SIZE); bitPos++) {
					// if nBit of to is enabled
					curBit = (uint.bitand (this.at[level], uint.bitflag (bitPos))) ? 1 : 0;
					// if nBit of from is enabled
					if (uint.bitand (from.at[level], uint.bitflag (bitPos))) curBit++;

					if (outBit !== 0) { outBit--; curBit++; }
					if (curBit > 1) { outBit++; curBit -= 2; }

					// enable or disable bit in to
					if (curBit === 1) this.at[level] = uint.bitor (this.at[level], uint.bitflag (bitPos));
					else this.at[level] = uint.bitnotand (this.at[level], uint.bitflag (bitPos));
				}
				// OUT OF BOUNDERS
				if (level === 0 && outBit !== 0) {
					this.Set_Zero();
					level = Constants.LEVEL_TOP;
					for (; outBit !== 0 && level > 0; level--)
					for (let bitPos = 1; uint.lesserequal (bitPos, Constants.INTEGER_SIZE); outBit--, bitPos++)
					this.at[level] = uint.bitor (this.at[level], uint.bitflag (bitPos));
					this.LvlButton_Configure();
					this.Sign_Change();
				}
				else if (this.at[level] !== 0) this.lvlButton = level;
			}
		}

		_sub_bbi_op (subtrahend) {
			// if subtrahend is system integer and out of bounders not possible then try simple language difference
			if (subtrahend.Check_IsSystemInteger() && this.Check_IsSystemInteger()
			&& uint.outofmin (this.at[Constants.LEVEL_TOP], subtrahend.at[Constants.LEVEL_TOP]) === false) {
				this.Sub_Level (subtrahend.at[Constants.LEVEL_TOP], Constants.LEVEL_TOP);
				return;
			}
			// OUT OF BOUNDERS
			else if (this.CompareUnsigned (subtrahend) === -1) {
				let temp = new bbi(subtrahend);
				temp._sub_bbi_op (this);
				temp.Sign_Set ((this.sign === false) ? true : false);
				this.Copy(temp);
				return;
			}
			else {
				let bitPos = 1;
				let level = Constants.LEVEL_TOP;
				let outBit = 0;
				let curBitSecond = 0;
				let curBitFirst = 0;

				for (; uint.biggerequal (level, this.lvlButton); level--)
				for (bitPos = 1; uint.lesserequal (bitPos, Constants.INTEGER_SIZE); bitPos++) {
					curBitFirst = uint.bitand (this.at[level], uint.bitflag (bitPos)) ? 1 : 0;
					curBitSecond = uint.bitand (subtrahend.at[level], uint.bitflag (bitPos)) ? 1 : 0;

					if (curBitFirst === curBitSecond) {
						if (outBit === 0) this.at[level] = uint.bitnotand (this.at[level], uint.bitflag (bitPos));
						else {
							this.at[level] = uint.bitor (this.at[level], uint.bitflag (bitPos));
							outBit = 0;
						}
					}
					else if (curBitSecond === 1) {
						if (outBit === 1) this.at[level] = uint.bitnotand (this.at[level], uint.bitflag (bitPos));
						else {
							this.at[level] = uint.bitor (this.at[level], uint.bitflag (bitPos));
							outBit = curBitSecond;
						}
					}
					else if (outBit !== 0) {
						this.at[level] = uint.bitnotand (this.at[level], uint.bitflag (bitPos));
						outBit = 0;
					}
				}
				this.LvlButton_Configure();
			}
		}

		// @ADD

		_add_LevelOut (integer, fromLvl, prevLvl) {
			if (prevLvl !== 0) {
				if (uint.outofmax(this.at[prevLvl-1],1) === true) {
					this._add_LevelOut (integer, fromLvl, prevLvl-1);
					return;
				}
				else
					this.at[prevLvl-1]++;
			}
			else if (prevLvl === 0) {
				let temp = new bbi();
				temp.at[fromLvl] = integer;
				temp.lvlButton = fromLvl;
				this._add_bbi_op (temp);
				return;
			}

			for (let curLvl = prevLvl; uint.lesser (curLvl, fromLvl); curLvl++)
				this.at[curLvl] = Constants.LEVEL_IS_EMPTY;
			// x = x + y
			// the number needed to overflow is z = FULL - y + 1
			// the value to set is x = EMPTY + x - z
			this.at[fromLvl] = 1 + uint.add (Constants.LEVEL_IS_EMPTY, uint.sub (this.at[fromLvl], uint.sub ( uint.sub(Constants.LEVEL_IS_FULL, integer))));
		}

		Add_Level (integer, level) {
			if (typeis (level, "number") && typeis (integer, "number")) {

				if (uint.outofmax (this.at[level], integer) === false)
					this.at[level] += integer;
				else
					this._add_LevelOut (integer, level, level);
			}
		}

		Add () {
			if (arguments.length !== 1) return;
			else if (typeis (arguments[0], "bbi")) {

				if (this.sign === arguments[0].sign)
					this._add_bbi_op (arguments[0]);
				else
					this._sub_bbi_op (arguments[0]);
			}
			else if (typeis (arguments[0], "number")) {

				if (this.sign === false)
					this.Add_Level (arguments[0], Constants.LEVEL_TOP);
				else if (this.sign === true)
					this.Sub_Level (arguments[0], Constants.LEVEL_TOP);
			}

		}

		Add_To (to, second) {
			if (typeis(to, "bbi") && typeis(second, "bbi")) {

				to.Copy (second);
				if (this.sign === second.sign)
					to._add_bbi_op(second);
				else
					to._sub_bbi_op(second);
			}
		}

		Add_New (second) {
			if (typeis (second, "bbi")) {

				let result = new bbi(this);
				if (this.sign === second.sign)
					result._add_bbi_op(second);
				else
					result._sub_bbi_op(second);
				return result;
			}
		}

		_sub_LevelOut (integer, fromLvl, prevLvl) {
			if (prevLvl !== 0) {
				if (uint.outofmin(this.at[prevLvl-1],1) === true) {
					_sub_LevelOut (integer, fromLvl, prevLvl-1);
					return;
				}
				else
					this.at[prevLvl-1]--;
			}
			else if (prevLvl === 0) {
				let temp = new bbi();
				temp.at[fromLvl] = integer;
				temp.lvlButton = fromLvl;
				this._sub_bbi_op (temp);
				return;
			}

			for (let curLvl = prevLvl; uint.lesser (curLvl, fromLvl); curLvl++)
				this.at[curLvl] = Constants.LEVEL_IS_FULL;
			// x = x - y
			// the number needed to overflow is z = EMPTY + y - 1
			// the value to set x = FULL - x + z
			this.at[fromLvl] = uint.sub (Constants.LEVEL_IS_FULL, uint.add (this.at[fromLvl], uint.add (Constants.LEVEL_IS_EMPTY, integer))) -1;

			for (let curLvl = prevLvl+1; curLvl <= Constants.LEVEL_TOP; curLvl++)
				this.at[curLvl] = this.sub (Constants.LEVEL_IS_FULL, this.at[curLvl]) + 1;
		}

		Sub_Level (integer, level) {
			if (typeis (level, "number") && typeis (integer, "number")) {

				if (uint.outofmin (this.at[level], integer) === false)
					this.at[level] -= integer;
				else
					this._sub_LevelOut (integer, level, level);
			}
		}

		Sub () {
			if (arguments.length !== 1) return;
			if (typeis (arguments[0], "bbi")) {

				if (this.sign === arguments[0].sign)
					this._sub_bbi_op (arguments[0]);
				else
					this._add_bbi_op (arguments[0]);
			}
			else if (typeis (arguments[0], "number")) {

				if (this.sign === false)
					this.Sub_Level (arguments[0], Constants.LEVEL_TOP);
				else if (this.sign === true)
					this.Add_Level (arguments[0], Constants.LEVEL_TOP);
			}
		}

		Sub_To (to, second) {
			if (typeis(to, "bbi") && typeis(second, "bbi")) {

				to.Copy (second);
				if (this.sign === second.sign)
					to._sub_bbi_op(second);
				else
					to._add_bbi_op(second);
			}
		}

		Sub_New (second) {
			if (typeis (second, "bbi")) {

				let result = new bbi(this);
				if (this.sign === second.sign)
					result._sub_bbi_op(second);
				else
					result._add_bbi_op(second);
				return result;
			}
		}

		// @MULTIPLICATION

		_multiplicationByBitAnd_op (temp) {

			// if (temp.Check_IsOne()) return;
			// if (temp.Check_IsZero()) {
				// this.Set_Zero();
				// return;
			// }

			// Integer x BBIA
			// 101 x 010
			// 101 x 01(0) === 101 & 000 = 000
			// 101 x 0(1)0 === 1010 & 1111 = 1010
			// 101 x (0)10 === 10100 & 00000 = 00000
			// 000 + 1010 + 00000 = 1010 (101 x 010)
			// See calculation folder for more information

			let multiplier = new bbi(this);
			let curBitVal = false;

			for (let i = 0; uint.lesserequal (i, Constants.LEVEL_TOP); i++) this.at[i] = Constants.LEVEL_IS_EMPTY;
			this.lvlButton = Constants.LEVEL_TOP;

			for (
				let curBit = 1, lvl = Constants.LEVEL_TOP,
				lvlBit = curBit % Constants.INTEGER_SIZE;

				uint.lesserequal (curBit, Constants.BITS_COUNT);

				curBit++, lvlBit = curBit % Constants.INTEGER_SIZE
			) {
				if (lvlBit !== 0)
					curBitVal = uint.bitand (multiplier.at[lvl], uint.bitflag (lvlBit)) ? true : false;
				else
					curBitVal = uint.bitand (multiplier.at[lvl--], uint.bitflag (Constants.INTEGER_SIZE)) ? true : false;

				if (curBitVal === true)
					this.Add (temp);

				temp.Bits_ShiftLeft (1);
			}
		}

		Mult_New () {
			if (arguments.length === 2 && typeis (arguments[0], "boolean") && typeis (arguments[1], "number") ) {

				let result = new bbi(this);
				result._multiplicationByBitAnd_op (new bbi (arguments[0], arguments[1]));
				if ((this.sign !== arguments[0]) || (this.sign === true && arguments[0] === true)) result.Sign_Change();
				return result;
			}
			else if (arguments.length === 1 && typeis (arguments[0], "bbi")) {

				// if (this.CompareUnsigned (arguments[0]) === 0)
					// return this.Pow_New (2);

				let result = new bbi(this);
				result._multiplicationByBitAnd_op (new bbi(arguments[0]));
				if ((this.sign !== arguments[0].sign) || (this.sign === true && arguments[0].sign === true)) result.Sign_Change();
				return result;
			}
		}

		Mult () {
			if (arguments.length === 2 && typeis (arguments[0], "boolean") && typeis (arguments[1], "number") ) {
				this._multiplicationByBitAnd_op (new bbi(arguments[0], arguments[1]));
				if ((this.sign !== arguments[0]) || (this.sign === true && sign === arguments[0])) this.Sign_Change();
			}
			else if (arguments.length === 1 && typeis (arguments[0], "bbi")) {
				let toChangeSign = ((this.sign !== arguments[0].sign) || (this.sign === true && arguments[0].sign === true)) ? true : false;
				this.Mult_Unsigned (arguments[0]);
				if (toChangeSign === true) this.Sign_Change();
			}
		}

		Mult_Unsigned (second) {
			if (typeis (second, "bbi")) {

				// if (this.CompareUnsigned (second) === 0) {
					// this.Pow (2);
					// return;
				// }
				this._multiplicationByBitAnd_op (new bbi (second));
			}
		}

		// @POWER

		_Pow (power) {
			if (typeis (power, "number")) {
				if (power === 0) {
					this.Set_Value(false,1);
					return;
				}

				let temp = new bbi(this);
				let saved = new bbi(this);

				while (power-- !== 1) {
					this.Mult (temp);
					temp.Copy (saved);
				}
			}
		}

		___Pow_old (first, power) {
			if (power <= 1) return;
			process.nextTick (() => {
				this.Mult (first);
				this.Pow (first, power--);
			});
		}

		Pow (power) {
			if (this.sign && power % 2 === 0) this.Sign_Change();
			this._Pow (power);
		}

		Pow_New (power) {
			let res = new bbi(this);
			res.Pow (this,power);
			return res;
		}

		// @DIVISION

		// enum DIVISION_OR_MOD : {DIVISION, MOD}
		// true === division, false === mod

		_divisionBy_op (division, flag) {

			if (division.Check_IsOne()) return;

			// set result to zero and work with it like with current multiplication of division
			let currentMultiplierOfDivision = new bbi();
			currentMultiplierOfDivision.at[Constants.LEVEL_TOP] = 2;

			// create temporary to store what must be subtrahended from mod
			let currentDifference = new bbi (division);

			for (; this.CompareUnsigned (currentDifference) === 1;
			       currentMultiplierOfDivision.Add (1) )
			{
				currentDifference.Copy (division);
				currentDifference.Mult_Unsigned (currentMultiplierOfDivision);
			}

			currentMultiplierOfDivision.Sub (1); // analog for i--
			// division result
			if (flag === true) {
				this.Copy(currentMultiplierOfDivision);
				return;
			}

			// mod result
			else if (flag === false) {
				currentDifference.Copy (division);
				currentDifference.Mult_Unsigned (currentMultiplierOfDivision);
				this.Sub (currentDifference);
			}
		}

		Div () {
			if (arguments.length === 2 && typeis (arguments[0], "boolean") && typeis (arguments[1], "number") ) {
				if (arguments[1] === Constants.LEVEL_IS_EMPTY) {return;}
				this.Div (new bbi(arguments[0], arguments[1]));
			}
			else if (arguments.length === 1 && typeis (arguments[0], "bbi")) {

				if (arguments[0].Check_IsZero() === true) {return;}
				let toChangeSign = ((this.sign !== arguments[0].sign) || (this.sign === true && arguments[0].sign === true)) ? true : false;
				this._divisionBy_op (arguments[0], true);
				this.LvlButton_Configure();
				if (toChangeSign === true) this.Sign_Change();
			}
		}

		Div_New () {
			if (arguments.length === 2 && typeis (arguments[0], "boolean") && typeis (arguments[1], "number") ) {
				let res = new bbi(this);
				res.Div (arguments[0], arguments[1]);
				return res;
			}
			else if (arguments.length === 1 && typeis (arguments[0], "bbi")) {
				let res = new bbi(this);
				res.Div (arguments[0]);
				return res;
			}
		}

		// @MOD

		Mod () {
			if (arguments.length === 1 && typeis (arguments[0], "number") ) {
				if (integer === Constants.LEVEL_IS_EMPTY) {return;}
				this.Mod (new bbi(false, arguments[0]));
			}
			else if (arguments.length === 1 && typeis (arguments[0], "bbi")) {
				if (division.Check_IsZero() === true) {return;}
				divided._divisionBy_op (arguments[0], false);
				divided.LvlButton_Configure();
			}
		}

		Mod_New () {
			if (arguments.length === 1 && typeis (arguments[0], "number") ) {
				let res = new bbi(this);
				res.Mod (arguments[0]);
				return res;
			}
			else if (arguments.length === 1 && typeis (arguments[0], "bbi")) {
				let res = new bbi(this);
				res.Mod (arguments[0]);
				return res;
			}
		}

		// @LOGARITHM

		Log () {
			if (this.Check_IsOne()) {
				self.Set_Zero();
				return;
			}
			if (arguments.length === 2 && typeis (arguments[0], "boolean") && typeis (arguments[1], "number") ) {
				if (this.CompareUnsigned (arguments[1]) === -1) {
					return;
				}
				if (this.Check_IsZero() || arguments[1] === 0) {
					return;
				}

				let curPower = 1;
			 	let compare = new bbi (arguments[0], arguments[1]);
				while (this.CompareUnsigned (compare) === 1) {
					compare.Set_SystemInteger (arguments[0], arguments[1]);
					compare.Pow (++curPower);
				}
				this.Set_SystemInteger ((arguments[0] === true && curPower % 2 !== 0) ? true : false, curPower);
			}
			else if (arguments.length === 1 && typeis (arguments[0], "bbi")) {
				if (this.CompareUnsigned (arguments[0]) === -1) {
					return;
				}
				if (this.Check_IsZero() || arguments[0].Check_IsZero()) {
					return;
				}

				let curPower = 1;
			 	let compare = new bbi(arguments[0]);

				while (this.CompareUnsigned (compare) === 1) {
					compare.Copy (arguments[0]);
					compare.Pow (++curPower);
				}
				this.Set_SystemInteger ((arguments[0].sign === true && curPower % 2 !== 0) ? true : false, curPower);
			}
		}

		Log_New () {
			if (this.Check_IsOne()) {
				return new bbi();
			}
			if (arguments.length === 2 && typeis (arguments[0], "boolean") && typeis (arguments[1], "number") ) {
				if (this.CompareUnsigned (arguments[1]) === -1) {
					return new bbi();
				}
				if (this.Check_IsZero() || arguments[1] === 0) {
					return new bbi();
				}

				let curPower = 1;
				let compare = new bbi (arguments[0], arguments[1]);
				while (this.CompareUnsigned (compare) === 1) {
					compare.Set_SystemInteger (arguments[0], arguments[1]);
					compare.Pow (++curPower);
				}
				compare.Set_SystemInteger ((arguments[0] === true && curPower % 2 !== 0) ? true : false, curPower);
				return compare;
			}
			else if (arguments.length === 1 && typeis (arguments[0], "bbi")) {
				if (this.CompareUnsigned (arguments[0]) === -1) {
					return new bbi();
				}
				if (this.Check_IsZero() || arguments[0].Check_IsZero()) {
					return new bbi();
				}

				let curPower = 1;
			 	let compare = new bbi(arguments[0]);

				while (this.CompareUnsigned (compare) === 1) {
					compare.Copy (arguments[0]);
					compare.Pow (++curPower);
				}
				compare.Set_SystemInteger ((arguments[0].sign === true && curPower % 2 !== 0) ? true : false, curPower);
				return compare;
			}
		}

		// SQRT

		Sqrt () {
		 	let compare = new bbi(this);
			let temp = this.Pow_New(2);

			while (compare.CompareUnsigned(temp) === -1) {
				// x / r
				temp = compare.Div_New(this);
				// r + x / r
				this.Add (temp);
				// (r + x / r) / 2
				this.Bits_ShiftRight (1);
				// temp = r*r
				temp = this.Pow_New(2);
			}
		}

		Sqrt_New () {
		 	let compare = new bbi(this);
			let temp = compare.Pow_New(2);

			while (this.CompareUnsigned(temp) === -1) {
				// x / r
				temp = this.Div_New(compare);
				// r + x / r
				compare.Add (temp);
				// (r + x / r) / 2
				compare.Bits_ShiftRight (1);
				// temp = r*r
				temp = compare.Pow_New(2);
			}
			return compare;
		}

		toBaseOfTwo (base) {
			if (typeis (base, "number") === false) return "+0";
			let powerOfTwo = uint.inBaseOfTwo (base);
			if (powerOfTwo === -2) return "+0";

			let position =
			 (this.at[this.lvlButton] < 0) ?
			1 // for sign in zero position
			+ uint.log2 (this.at[this.lvlButton]) / Math.log2 (base)
			+ Math.ceil (Constants.INTEGER_SIZE/powerOfTwo) * (Constants.LEVEL_TOP-this.lvlButton)
			:
			1
			+ Math.log2 (this.at[this.lvlButton]) / Math.log2 (base)
			+ Math.ceil (Constants.INTEGER_SIZE/powerOfTwo) * (Constants.LEVEL_TOP-this.lvlButton);

			// console.log (position);
			position |= 0;
			let reverse = "";

			for (
				let curLvl = Constants.LEVEL_TOP,
				curBit = 0,
				curBitInTwo = 0,
				curDigit = 0;

				uint.bigger (position, 0);

				position--
			)
			{
				for (curBitInTwo = 1; curBitInTwo <= powerOfTwo; curBitInTwo++) {

					if (curBit === Constants.INTEGER_SIZE) {
						curBit = 0;
						curLvl--;
					}

					curDigit =
					uint.bitor (
					curDigit,
					uint.bitand (uint.bitflag (++curBit), this.at[curLvl])
					? uint.bitflag (curBitInTwo)
					: 0
					);
				}

				reverse += uint.numerics.split("")[curDigit];
				curDigit = 0;
			}

			reverse += (this.sign === false) ? '+' : '-';

			return reverse.split("").reverse().join("");
		}

		fromBaseOfTwo (str, base) {
			if ( !(typeis (str, "string") && typeis (base, "number")) ) return;
			let powerOfTwo = uint.inBaseOfTwo (base);
			if (powerOfTwo === -2 || uint.validate_inBase (str) === false) return;

			this.Set_Zero();
			this.Sign_Set (str.charCodeAt(0) === '-');

			let position = str.length-1;

			for (
				let curLvl = Constants.LEVEL_TOP,
				curBit = 0,
				curBitInTwo = 0,
				curDigit = 0;

				position > 0;

				position--
			)
			{
				curDigit = uint.numerics.indexOf(str.split('')[position]);

				for (curBitInTwo = 1; curBitInTwo <= powerOfTwo; curBitInTwo++) {

					if (curBit === Constants.INTEGER_SIZE) {
						curBit = 0;
						curLvl--;
					}

					if (uint.bitand (uint.bitflag (curBitInTwo), curDigit))
						this.at[curLvl] = uint.bitor (this.at[curLvl], uint.bitflag (++curBit));
					else
						++curBit;
				}
			}
		}

		fromBaseOfTwo_new (str, base) {
			if ( !(typeis (str, "string") && typeis (base, "number")) ) return;
			let powerOfTwo = uint.inBaseOfTwo (base);
			if (powerOfTwo === -2 || uint.validate_inBase (str) === false) return;

			let result = new bbi();
			result.fromBaseOfTwo (str, base);
			return result;
		}
}

module.exports = {
	bbi
}
