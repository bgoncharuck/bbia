using System;

namespace bbi {

	public class Constants {

		public const byte INTEGER_SIZE = 32;

		/*SIZE*/
		public const uint BITS_512 = 512/INTEGER_SIZE ;// 2^9
		/*
		public const uint BITS_1024 = 1024/INTEGER_SIZE ;// 2^10
		public const uint BITS_2048 = 2048/INTEGER_SIZE ;// 2^11
		public const uint BITS_4096 = 4096/INTEGER_SIZE ;// 2^12
		public const uint BITS_8192 = 8192/INTEGER_SIZE ;// 2^13
		public const uint BITS_16384 = 16384/INTEGER_SIZE ;// 2^14
		public const uint BITS_32768 = 32768/INTEGER_SIZE ;// 2^15
		public const uint BITS_65536 = 65536/INTEGER_SIZE ;// 2^16
		public const uint BITS_131072 = 131072/INTEGER_SIZE ;// 2^17
		public const uint BITS_262144 = 262144/INTEGER_SIZE ;// 2^18
		public const uint BITS_524288 = 524288/INTEGER_SIZE ;// 2^19
		public const uint BITS_1048576 = 1048576/INTEGER_SIZE ;// 2^20
		public const uint BITS_2097152 = 2097152/INTEGER_SIZE ;// 2^21
		public const uint BITS_4194304 = 4194304/INTEGER_SIZE ;// 2^22
		public const uint BITS_8388608 = 8388608/INTEGER_SIZE ;// 2^23
		public const uint BITS_16777216 = 16777216/INTEGER_SIZE ;// 2^24
		public const uint BITS_33554432 = 33554432/INTEGER_SIZE ;// 2^25
		public const uint BITS_67108864 = 67108864/INTEGER_SIZE ;// 2^26
		public const uint BITS_134217728 = 134217728/INTEGER_SIZE ; // 2^27
		// WARNING : Be carefull with it. Needs a big amount of RAM.
		public const uint BITS_268435456 = 268435456/INTEGER_SIZE ;// 2^28
		public const uint BITS_536870912 = 536870912/INTEGER_SIZE ;// 2^29
		public const uint BITS_1073741824 = 1073741824/INTEGER_SIZE ;// 2^30
		public const uint BITS_2147483648 = 2147483648/INTEGER_SIZE ;// 2^31
		// WARNING : Be carefull with max value of 32bit bbia.
		// It can`t be tested on current computer.
		// 4294967296 but used 4294967295 to avoid overflow in some systems
		public const uint BITS_4294967296 = 4294967295/INTEGER_SIZE ; // 2^32 - 1
		*/

		/*LEVEL*/
		/*@CHOOSE size here*/
		public const uint LEVEL_COUNT = BITS_512;

		public const uint BITS_COUNT = LEVEL_COUNT * INTEGER_SIZE;
		public const uint LEVEL_TOP = -1 + LEVEL_COUNT;

		/*DIGIT*/
		// signed int 1111...n (n == INTEGER_SIZE)
		public const uint LEVEL_IS_FULL = uint.MaxValue;
		// signed int 0000...n (n == INTEGER_SIZE)
		public const uint LEVEL_IS_EMPTY = uint.MinValue;

		/*SIGN*/
		public const bool SIGNED = true;
		public const bool UNSIGNED = false;
	}

	// Bit Big Integer Exception
	public class Exceptions {

		[Serializable]
		public class BBIException : Exception {
			public BBIException () {}
			public BBIException (string type)
			: base (String.Format("BitBigInt Exception: {0}", type)){}
		}

		[Serializable]
		public class InvalidBitInBitflag : BBIException {
			public InvalidBitInBitflag () {}
			public InvalidBitInBitflag (byte bit)
			: base(String.Format("Invalid Bit In Bitflag {0} ; Must be from 1 to {1}", bit, Constants.INTEGER_SIZE)){}
		}

		public class DivisionByZero : BBIException {
			public DivisionByZero () {}
			public DivisionByZero (BitBigInt divided)
			: base(String.Format("A try to divide {0} by 0 ; Undefined operation.", divided.toBase(16)){}
		}

	}

	// stuaa, but C# have unsigned int
	public class uintArithmetics {

		public uint Bitflag (byte bit) {

			if ( !(bit >= 0 && bit <= Constants.INTEGER_SIZE) )
				throw new Exceptions.InvalidBitInBitflag(bit);

			switch (bit) {
				case 1 : return 0x1;
				case 2 : return 0x2;
				case 3 : return 0x4;
				case 4 : return 0x8;
				case 5 : return 0x10;
				case 6 : return 0x20;
				case 7 : return 0x40;
				case 8 : return 0x80;
				case 9 : return 0x100;
				case 10 : return 0x200;
				case 11 : return 0x400;
				case 12 : return 0x800;
				case 13 : return 0x1000;
				case 14 : return 0x2000;
				case 15 : return 0x4000;
				case 16 : return 0x8000;
				case 17 : return 0x10000;
				case 18 : return 0x20000;
				case 19 : return 0x40000;
				case 20 : return 0x80000;
				case 21 : return 0x100000;
				case 22 : return 0x200000;
				case 23 : return 0x400000;
				case 24 : return 0x800000;
				case 25 : return 0x1000000;
				case 26 : return 0x2000000;
				case 27 : return 0x4000000;
				case 28 : return 0x8000000;
				case 29 : return 0x10000000;
				case 30 : return 0x20000000;
				case 31 : return 0x40000000;
				case 32 : return 0x80000000;

				case 0 :
				default : return 0;
			}
			return 0;
		}

		public bool OutOfUint_Add (uint to, uint test) => return to > uint.MaxValue - test;

		public bool OutOfUint_Sub (uint from, uint test) => return from < uint.MinValue + test;

		public bool BaseOf2 (uint value) {
			for (uint curBit = 2; curBit <= Constants.INTEGER_SIZE; curBit++)
				if (value == Bitflag (curBit))
					return true;
			return false;
		}

		public sbyte Comapre (uint a, uint b) => return (a == b) ? 0 : (a > b) ? 1 : -1;
	}

	public class BitBigInt {

		// FIELDS
		public uint[] at;
		public bool sign;
		public uint lvlButton;

		public BitBigInt () {
			this.at = new uint[Constants.LEVEL_COUNT];
			this.sign = false;
			this.lvlButton = Constants.LEVEL_TOP;
		}

		// from system integer
		public BitBigInt (bool sign, uint integer) {
			this.at = new uint[Constants.LEVEL_COUNT];
			this.sign = sign;
			this.lvlButton = Constants.LEVEL_TOP;
			this.at[Constants.LEVEL_TOP] = integer;
		}
		// copy new
		public BitBigInt (BitBigInt copyFrom) {
			this.at = new uint[Constants.LEVEL_COUNT];
			this.Copy (copyFrom);
		}

		public void Sign_Change () => this.sign = (this.sign) ? false : true;
		public void Sign_Set (bool newSign) => this.sign = newSign;

		public void LvlButton_Configure () {
			for (uint curLvl = 0; curLvl < Constants.LEVEL_TOP; curLvl++)
				if (this.at[curLvl] != Constants.LEVEL_IS_EMPTY) {
					this.lvlButton = curLvl;
					return;
				}
			this.lvlButton = Constants.LEVEL_TOP;
		}
		public void LvlButton_Set (uint level) => this.lvlButton = level;

		private void _setters_op (uint val, bool sign) {
			if (val != Constants.LEVEL_IS_EMPTY) {
				for (uint curLvl = 0; curLvl <= Constants.LEVEL_TOP; curLvl++) this.at[curLvl] = val;
				this.sign = sign;
				this.lvlButton = 0;
			}
			else {
				for (uint curLvl = 0; curLvl <= Constants.LEVEL_TOP; curLvl++) this.at[curLvl] = Constants.LEVEL_IS_EMPTY;
				this.sign = false;
				this.lvlButton = Constants.LEVEL_TOP;
			}
		}
		public void Set_Zero () => this._setters_op (Constants.LEVEL_IS_EMPTY, false);
		public void Set_Min () => this._setters_op (Constants.LEVEL_IS_FULL, true);
		public void Set_Max () => this._setters_op (Constants.LEVEL_IS_FULL, false);
		public void Set_Value (uint value) => this._setters_op (value, false);
		public void Set_ValueSigned (uint value) => this._setters_op (value, true);

		public void Set_ValueFromLevel (uint level, uint value) {
			for (uint curLvl = 0; curLvl <= level; curLvl++)
				this.at[curLvl] = value;
			this.sign = false;
			this.lvlButton = 0; if (value == Constants.LEVEL_IS_EMPTY) this.LvlButton_Configure();
		}

		public void Set_ValueFromLevelSigned (uint level, uint value) {
			this.Set_ValueFromLevel (level,value);
			this.sign = true;
		}

		public void Set_ValueToLevel (uint level, uint value) {
			for (uint curLvl = level; curLvl <= Constants.LEVEL_TOP; curLvl++)
				this.at[curLvl] = value;
			this.sign = false;
			this.lvlButton = level; if (value == Constants.LEVEL_IS_EMPTY) this.LvlButton_Configure();
		}

		public void Set_ValueToLevelSigned (uint level, uint value) {
			this.Set_ValueToLevel (level,value);
			this.sign = true;
		}

		public void Set_SystemInteger (bool isSigned, uint integer) {
			this.Set_Zero();
			this.at[Constants.LEVEL_TOP] = integer;
			this.sign = isSigned;
		}


		public bool Check_IsInteger (uint integer) {
			if (this.lvlButton < Constants.LEVEL_TOP) return false;
			return (this.at[Constants.LEVEL_TOP] == integer) ? true : false;
		}
		public bool Check_IsZero () => return this.Check_IsInteger (0);
		public bool Check_IsOne () => return this.Check_IsInteger (1);
		public bool Check_IsSystemInteger () => return (this.lvlButton == Constants.LEVEL_TOP) ? true : false;

		public sbyte CompareUnsigned (BitBigInt b) {
			sbyte curCompare = 0;
			int curLvl = (this.lvlButton <= b.lvlButton) ? this.lvlButton : b.lvlButton;
			for (; curLvl <= Constants.LEVEL_TOP; curLvl++) {
				curCompare = uintArithmetics.Compare (this.at[curLvl], b.at[curLvl]);
				if (curCompare != 0)
					return curCompare;
			}
			return 0;
		}

		public sbyte Compare (BitBigInt b) {
			sbyte res = this.Compare (b);

			if (this.sign == b.sign) return res;
			else if (this.sign == true) return -1;
			else if (b.sign == true) return 1;
			return 0;
		}

		public sbyte CompareUnsigned (uint toCompare) {
			if (this.lvlButton < Constants.LEVEL_TOP) return 1;
			return uintArithmetics.Compare (this.at[Constants.LEVEL_TOP], toCompare);
		}

		public sbyte Compare (bool isSigned, uint toCompare) {
			sbyte res = this.CompareUnsigned (toCompare);

			if (this.sign == isSigned) return res;
			else if (this.sign == true) return -1;
			else if (isSigned == true) return 1;
			return 0;
		}

		public void Copy (BitBigInt from) {
			for (uint curLvl = 0; curLvl <= Constants.LEVEL_TOP; curLvl++)
				this.at[curLvl] = from.at[curLvl];
			this.sign = from.sign;
			this.lvlButton = from.lvlButton;
		}

		// @BIT_OPERATIONS

		public void Bits_ShiftLeft (uint value) {

			uint savedBits [Constants.LEVEL_TOP];
			uint bitMask[2] = {0,0};
			for (uint curBit = Constants.INTEGER_SIZE-value+1; curBit <= Constants.INTEGER_SIZE; curBit++)
				bitMask[0] |= uintArithmetics.Bitflag(curBit);
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
			for (uint lvl = Constants.LEVEL_TOP; lvl > this.lvlButton; this.at[lvl] <<= value, lvl--) {
				bitMask[1] = this.at[lvl] & bitMask[0];
				bitMask[1] >>= Constants.INTEGER_SIZE-value;
				savedBits[lvl-1] = bitMask[1];
			}
			this.at[this.lvlButton] <<= value;

			for (uint lvl = this.lvlButton; lvl < Constants.LEVEL_TOP; lvl++)
				this.at[lvl] |= savedBits[lvl];
		}

		public void Bits_ShiftRight (uint value) {

			uint savedBits [Constants.LEVEL_TOP];
			uint bitMask[2] = {0,0};
			for (uint curBit = 1; curBit <= value; curBit++)
				bitMask[0] |= uintArithmetics.Bitflag(curBit);
			// 1. Shift with saved bits
			// we shift all levels from zero to top
			// but save the part which is lost
			// we saved bits in position 1...value
			// but need set bits in position INTEGER_SIZE-value...INTEGER_SIZE
			// top level not count in saving

			// 2. Set saved bits
			// for all levels bigger then 0
			// we set saved bits
			for (uint lvl = this.lvlButton; lvl < Constants.LEVEL_TOP; this.at[lvl] >>= value, lvl++) {
				bitMask[1] = this.at[lvl] & bitMask[0];
				bitMask[1] <<= Constants.INTEGER_SIZE-value;
				savedBits[lvl] = bitMask[1];
			}
			this.at[Constants.LEVEL_TOP] >>= value;

			for (uint lvl = this.lvlButton; lvl < Constants.LEVEL_TOP; lvl++)
				this.at[lvl+1] |= savedBits[lvl];
			if (this.at[this.lvlButton] == Constants.LEVEL_IS_EMPTY) this.lvlButton++;
		}

		public void Bits_Or (BitBigInt second) {
			this.lvlButton = (this.lvlButton <= second.lvlButton) ? this.lvlButton : second.lvlButton;
			for (uint curLvl = this.lvlButton; curLvl <= Constants.LEVEL_TOP; curLvl++)
				this.at[curLvl] |= second.at[curLvl];
		}

		public BitBigInt Bits_Or_New (BitBigInt second) {
			BitBigInt result = new BitBigInt(this);
			result.Bits_Or(second);
			return result;
		}

		public void Bits_And (BitBigInt second) {
			this.lvlButton = (this.lvlButton <= second.lvlButton) ? this.lvlButton : second.lvlButton;
			for (uint curLvl = this.lvlButton; curLvl <= Constants.LEVEL_TOP; curLvl++)
				this.at[curLvl] &= second.at[curLvl];
		}

		public BitBigInt Bits_And_New (BitBigInt second) {
			BitBigInt result = new BitBigInt(this);
			result.Bits_And(second);
			return result;
		}

		public void Bits_FlagSet (uint bit) {
			uint lvl = Constants.LEVEL_TOP - bit / Constants.INTEGER_SIZE;
			bit %= Constants.INTEGER_SIZE;
			if (bit != 0)
				this.at[lvl] |= uintArithmetics.Bitflag (bit);
			else
				this.at[lvl+1] |= uintArithmetics.Bitflag (Constants.INTEGER_SIZE);
			this.LvlButton_Configure();
		}

		public void Bits_FlagUnset (uint bit) {
			uint lvl = Constants.LEVEL_TOP - bit / Constants.INTEGER_SIZE;
			bit %= Constants.INTEGER_SIZE;
			if (bit != 0)
				this.at[lvl] &= ~uintArithmetics.Bitflag (bit);
			else
				this.at[lvl+1] &= ~uintArithmetics.Bitflag (Constants.INTEGER_SIZE);
			this.LvlButton_Configure();
		}

		public void Bits_FlagSetMult (uint[] bitFlagArray) {
			for (uint curFlag = 0; curFlag < bitFlagArray.length; curFlag++)
				this.Bits_FlagSet (bitFlagArray[curFlag]);
		}

		public void Bits_FlagUnsetMult (uint[] bitFlagArray) {
			for (uint curFlag = 0; curFlag < bitFlagArray.length; curFlag++)
				this.Bits_FlagUnset (bitFlagArray[curFlag]);
		}

		public BitBigInt Bits_Flag (uint bit) {
			BitBigInt result = new BitBigInt();
			uint lvl = Constants.LEVEL_TOP - bit / Constants.INTEGER_SIZE;
			bit %= Constants.INTEGER_SIZE;
			if (bit != 0)
				result.at[lvl] |= uintArithmetics.Bitflag (bit);
			else
				result.at[lvl+1] |= uintArithmetics.Bitflag (Constants.INTEGER_SIZE);

			result.LvlButton_Configure();
			return result;
		}

		public BitBigInt Bits_Full () {
			BitBigInt result = new BitBigInt();
			for (uint curLvl = 0; curLvl <= Constants.LEVEL_TOP; curLvl++)
				result.at[curLvl] = Constants.LEVEL_IS_FULL;
			result.lvlButton = 0;
			return result;
		}
		public BitBigInt Bits_Empty () => return new BitBigInt();

		public BitBigInt Bits_FullTillBit (uint bit) {
			BitBigInt result = new BitBigInt();
			uint lvl = Constants.LEVEL_TOP - bit / Constants.INTEGER_SIZE;
			bit %= Constants.INTEGER_SIZE;
			if (bit == 0) lvl++;

			for (uint curLvl = lvl + 1; curLvl < Constants.LEVEL_TOP; curLvl--)
				result.at[curLvl] = Constants.LEVEL_IS_FULL;
			for (uint curBit = 1; curBit < bit; curBit++)
				result.at[lvl] |= uintArithmetics.Bitflag (curBit);
			this.LvlButton_Configure();
			return result;
		}

		public BitBigInt Bits_EmptyTillBit (uint bit) {
			BitBigInt result = new BitBigInt();
			uint lvl = Constants.LEVEL_TOP - bit / Constants.INTEGER_SIZE;
			bit %= Constants.INTEGER_SIZE;
			if (bit == 0) lvl++;

			for (uint curLvl = lvl + 1; curLvl < Constants.LEVEL_TOP; curLvl--)
				result.at[curLvl] = Constants.LEVEL_IS_EMPTY;
			for (uint curBit = 1; curBit < bit; curBit++)
				result.at[lvl] &= ~(uintArithmetics.Bitflag (curBit));
			this.LvlButton_Configure();
			return result;
		}

		// ADD/SUB private module operations

		internal protected void _add_bbi_op (BitBigInt from) {
			// if from is system integer and out of bounders not possible then try simple language addition
			if (from.Check_IsSystemInteger() && this.at[0] < Constants.LEVEL_IS_FULL) {
				this.Add_Level (from.at[Constants.LEVEL_TOP], Constants.LEVEL_TOP);
				return;
			}
			else {
				this.lvlButton = (this.lvlButton <= from.lvlButton) ? this.lvlButton : from.lvlButton;
				uint level = Constants.LEVEL_TOP;
				register byte outBit = 0;
				register byte curBit = 0;

				// standart addition algorithm for binary N-bit integer
				for (; (level >= this.lvlButton) || (outBit != 0 && level > 0); level--)
				for (uint bitPos = 1; bitPos <= Constants.INTEGER_SIZE; bitPos++) {
					// if nBit of to is enabled
					curBit = (this.at[level] & uintArithmetics.Bitflag (bitPos)) ? 1 : 0;
					// if nBit of from is enabled
					if (from.at[level] & uintArithmetics.Bitflag (bitPos)) curBit++;

					if (outBit != 0) { outBit--; curBit++; }
					if (curBit > 1) { outBit++; curBit -= 2; }

					// enable or disable bit in to
					if (curBit == 1) this.at[level] |= uintArithmetics.Bitflag (bitPos);
					else this.at[level] &= ~(uintArithmetics.Bitflag (bitPos));
				}
				// OUT OF BOUNDERS
				if (level == 0 && outBit != 0) {
					this.Set_Zero();
					level = Constants.LEVEL_TOP;
					for (; outBit != 0 && level > 0; level--)
					for (uint bitPos = 1; bitPos <= Constants.INTEGER_SIZE; outBit--, bitPos++)
					this.at[level] |= uintArithmetics.Bitflag (bitPos);
					this.LvlButton_Configure();
					this.Sign_Change();
				}
				else if (this.at[level] != 0) this.lvlButton = level;
			}
		}

		// @TODO must not be accessed within class
		internal protected void _sub_bbi_op (BitBigInt subtrahend) {
			// if subtrahend is system integer and out of bounders not possible then try simple language difference
			if (subtrahend.Check_IsSystemInteger() && this.Check_IsSystemInteger()
			&& uintArithmetics.OutOfUint_Sub (this.at[Constants.LEVEL_TOP], subtrahend.at[Constants.LEVEL_TOP]) == false) {
				this.Sub_Level (subtrahend.at[Constants.LEVEL_TOP], Constants.LEVEL_TOP);
				return;
			}
			// OUT OF BOUNDERS
			else if (this.CompareUnsigned (subtrahend) == -1) {
				BitBigInt temp = new BitBigInt(subtrahend);
				// @TODO private method use
				temp._sub_bbi_op (this);
				temp.Sign_Set ((this.sign == false) ? true : false);
				this.Copy(temp);
				return;
			}
			else {
				uint bitPos = 1;
				uint level = Constants.LEVEL_TOP;
				register byte outBit = 0;
				register byte curBitSecond = 0;
				register byte curBitFirst = 0;

				for (; level >= this.lvlButton; level--)
				for (bitPos = 1; bitPos <= Constants.INTEGER_SIZE; bitPos++) {
					curBitFirst = (this.at[level] & uintArithmetics.Bitflag (bitPos)) ? 1 : 0;
					curBitSecond = (subtrahend.at[level] & uintArithmetics.Bitflag (bitPos)) ? 1 : 0;

					if (curBitFirst == curBitSecond) {
						if (outBit == 0) this.at[level] &= ~(uintArithmetics.Bitflag (bitPos));
						else {
							this.at[level] |= uintArithmetics.Bitflag (bitPos);
							outBit = 0;
						}
					}
					else if (curBitSecond == 1) {
						if (outBit == 1) this.at[level] &= ~(uintArithmetics.Bitflag (bitPos));
						else {
							this.at[level] |= uintArithmetics.Bitflag (bitPos);
							outBit = curBitSecond;
						}
					}
					else if (outBit != 0) {
						this.at[level] &= ~(uintArithmetics.Bitflag (bitPos));
						outBit = 0;
					}
				}
				this.LvlButton_Configure();
			}
		}

		// @ADD

		private void Add_LevelOut (uint integer, uint fromLvl, uint prevLvl) {
			if (prevLvl != 0) {
				if (uintArithmetics.OutOfUint_Add(this.at[prevLvl-1],1) == true) {
					this.Add_LevelOut (integer, fromLvl, prevLvl-1);
					return;
				}
				else
					this.at[prevLvl-1]++;
			}
			else if (prevLvl == 0) {
				BitBigInt temp = new BitBigInt();
				temp.at[fromLvl] = integer;
				temp.lvlButton = fromLvl;
				this._add_bbi_op (temp);
				return;
			}

			for (uint curLvl = prevLvl; curLvl < fromLvl; curLvl++)
				this.at[curLvl] = Constants.LEVEL_IS_EMPTY;
			// x = x + y
			// the number needed to overflow is z = FULL - y + 1
			// the value to set is x = EMPTY + x - z
			this.at[fromLvl] = Constants.LEVEL_IS_EMPTY + this.at[fromLvl] - (Constants.LEVEL_IS_FULL - integer + 1);
		}

		public void Add_Level (uint integer, uint level) {
			if (uintArithmetics.OutOfUint_Add (this.at[level], integer) == false)
				this.at[level] += integer;
			else
				this.Add_LevelOut (integer, level, level);
		}

		public void Add (uint integer) {
			if (this.sign == false)
				this.Add_Level (integer, Constants.LEVEL_TOP);
			else if (this.sign == true)
				this.Sub_Level (integer, Constants.LEVEL_TOP);
		}

		public void Add (BitBigInt second) {
			if (this.sign == second.sign)
				this._add_bbi_op (second);
			else
				this._sub_bbi_op (second);
		}

		public void Add_To (BitBigInt to, BitBigInt second) {
			to.Copy (second);
			if (this.sign == second.sign)
				to._add_bbi_op(second);
			else
				to._sub_bbi_op(second);
		}

		public BitBigInt Add_New (BitBigInt second) {
			BitBigInt result = new BitBigInt(this);
			if (this.sign == second.sign)
				result._add_bbi_op(second);
			else
				result._sub_bbi_op(second);
			return result;
		}

		// SUBTRAHEND

		private void Sub_LevelOut (uint integer, uint fromLvl, uint prevLvl) {
			if (prevLvl != 0) {
				if (uintArithmetics.OutOfUint_Sub(this.at[prevLvl-1],1) == true) {
					Sub_LevelOut (integer, fromLvl, prevLvl-1);
					return;
				}
				else
					this.at[prevLvl-1]--;
			}
			else if (prevLvl == 0) {
				BitBigInt temp = new BitBigInt();
				temp.at[fromLvl] = integer;
				temp.lvlButton = fromLvl;
				this._sub_bbi_op (temp);
				return;
			}

			for (uint curLvl = prevLvl; curLvl < fromLvl; curLvl++)
				this.at[curLvl] = Constants.LEVEL_IS_FULL;
			// x = x - y
			// the number needed to overflow is z = EMPTY + y - 1
			// the value to set x = FULL - x + z
			this.at[fromLvl] = Constants.LEVEL_IS_FULL - this.at[fromLvl] + (Constants.LEVEL_IS_EMPTY + integer - 1);

			for (uint curLvl = prevLvl+1; curLvl <= Constants.LEVEL_TOP; curLvl++)
				this.at[curLvl] = Constants.LEVEL_IS_FULL - this.at[curLvl] + 1;
		}

		public void Sub_Level (uint integer, uint level) {
			if (uintArithmetics.OutOfUint_Sub (this.at[level], integer) == false)
				this.at[level] -= integer;
			else
				this.Sub_LevelOut (integer, level, level);
		}

		public void Sub (uint integer) {
			if (this.sign == false)
				this.Sub_Level (integer, Constants.LEVEL_TOP);
			else if (this.sign == true)
				this.Add_Level (integer, Constants.LEVEL_TOP);
		}

		public void Sub (BitBigInt second) {
			if (this.sign == second.sign)
				this._sub_bbi_op (second);
			else
				this._add_bbi_op (second);
		}

		public void Sub_To (BitBigInt to, BitBigInt second) {
			to.Copy (second);
			if (this.sign == second.sign)
				to._sub_bbi_op(second);
			else
				to._add_bbi_op(second);
		}

		public BitBigInt Sub_New (BitBigInt second) {
			BitBigInt result = new BitBigInt(this);
			if (this.sign == second.sign)
				result._sub_bbi_op(second);
			else
				result._add_bbi_op(second);
			return result;
		}

		// @MULTIPLICATION

		private void _multiplicationByBitAnd_op (BitBigInt temp) {

			// Integer x BBIA
			// 101 x 010
			// 101 x 01(0) == 101 & 000 = 000
			// 101 x 0(1)0 == 1010 & 1111 = 1010
			// 101 x (0)10 == 10100 & 00000 = 00000
			// 000 + 1010 + 00000 = 1010 (101 x 010)
			// See calculation folder for more information

			BitBigInt multiplier = new BitBigInt(this);
			bool curBitVal = false;

			for (uint i = 0; i <= Constants.LEVEL_TOP; i++) this.at[i] = Constants.LEVEL_IS_EMPTY;
			this.lvlButton = Constants.LEVEL_TOP;

			for (
				uint curBit = 1, lvl = Constants.LEVEL_TOP,
				lvlBit = curBit % Constants.INTEGER_SIZE;

				curBit <= Constants.BITS_COUNT;

				curBit++, lvlBit = curBit % Constants.INTEGER_SIZE
			) {
				if (lvlBit != 0)
					curBitVal = (multiplier.at[lvl] & uintArithmetics.Bitflag (lvlBit)) ? true : false;
				else
					curBitVal = (multiplier.at[lvl--] & uintArithmetics.Bitflag (Constants.INTEGER_SIZE)) ? true : false;

				if (curBitVal == true)
					this.Add (temp);

				temp.Bits_ShiftLeft (1);
			}
		}

		public BitBigInt Mult_New (bool sign, uint integer) {
			BitBigInt result = new BitBigInt(this);
			result._multiplicationByBitAnd_op (new BitBigInt (sign, integer));
			if ((this.sign != sign) || (this.sign == true && sign == true)) result.Sign_Change();
			return result;
		}

		public void Mult (bool sign, uint integer) {
			this._multiplicationByBitAnd_op (new BitBigInt(sign, integer));
			if ((this.sign != sign) || (this.sign == true && sign == true)) this.Sign_Change();
		}

		BitBigInt Mult_New (BitBigInt second) {
			if (this.CompareUnsigned (second) == 0)
				return this.Pow_New (2);

			BitBigInt result = new BitBigInt(this);
			result._multiplicationByBitAnd_op (new BitBigInt(second));
			if ((this.sign != second.sign) || (this.sign == true && second.sign == true)) result.Sign_Change();
			return result;
		}

		// @MULT BBI BBI
		public void Mult_Unsigned (BitBigInt second) {
			if (this.CompareUnsigned (second) == 0) {
				this.Pow (2);
				return;
			}
			this._multiplicationByBitAnd_op (new BitBigInt (second) );
		}

		public void Mult (BitBigInt second) {
			bool toChangeSign = ((this.sign != second.sign) || (this.sign == true && second.sign == true)) ? true : false;
			this.Mult_Unsigned (second);
			if (toChangeSign == true) this.Sign_Change();
		}

		// @POWER

		public void Pow (uint power) {
			BitBigInt temp = new BitBigInt(this);
			BitBigInt saved = new BitBigInt(this);

			while (power-- != 1) {
				this.Mult (temp);
				temp.Copy (saved);
			}
		}

		public void Pow_Signed (uint power) {
			if (this.sign && power % 2 == 0) this.Sign_Change();
			this.Pow (power);
		}

		public BitBigInt Pow_New (uint power) {
			BitBigInt res = new BitBigInt(this);
			res.Pow_Signed (power);
			return res;
		}

		// @DIVISION

		private enum DIVISION_OR_MOD : byte {DIVISION, MOD};

		private void _divisionBy_op (BitBigInt division, DIVISION_OR_MOD flag) {

			// set result to zero and work with it like with current multiplication of division
			BitBigInt currentMultiplierOfDivision = new BitBigInt();
			currentMultiplierOfDivision.at[Constants.LEVEL_TOP] = 2;

			// create temporary to store what must be subtrahended from mod
			BitBigInt currentDifference = new BitBigInt (division);

			for (; this.CompareUnsigned (currentDifference) == 1;
			       currentMultiplierOfDivision.Add (1) /*analog for i++)*/)
			{
				currentDifference.Copy (division);
				currentDifference.Mult_Unsigned (currentMultiplierOfDivision);
			}

			currentMultiplierOfDivision.Sub (1); // analog for i--
			// division result
			if (flag == DIVISION) {
				this.Copy(currentMultiplierOfDivision);
				return;
			}

			// mod result
			else if (flag == MOD) {
				currentDifference.Copy (division);
				currentDifference.Mult_Unsigned (currentMultiplierOfDivision);
				this.Sub (currentDifference);
			}
		}

		public void Div (BitBigInt division) {
			if (division.Check_IsZero() == true) {
				throw new Exceptions.DivisionByZero(this);
				return;
			}
			bool toChangeSign = ((this.sign != division.sign) || (this.sign == true && division.sign == true)) ? true : false;
			this._divisionBy_op (division, DIVISION);
			this.LvlButton_Configure();
			if (toChangeSign == true) this.Sign_Change();
		}

		BitBigInt Div_New (BitBigInt division) {
			BitBigInt res = new BitBigInt (this);
			res.Div (division);
			return res;
		}

		public void Div (bool signed, uint integer) {
			if (integer == Constants.LEVEL_IS_EMPTY) {
				throw new Exceptions.DivisionByZero(this);
				return;
			}
			this.Div (new BitBigInt (signed, integer));
		}

		BitBigInt Div_New (bool sign, uint integer) {
			BitBigInt res = new BitBigInt (this);
			res.Div (sign, integer);
			return res;
		}

		// @MOD

		public void Mod (BitBigInt division) {
			if (division.Check_IsZero() == true) {
				throw new Exceptions.DivisionByZero(this);
				return;
			}
			divided._divisionBy_op (division, MOD);
			divided.LvlButton_Configure();
		}

		public BitBigInt Mod_New (BitBigInt division) {
			BitBigInt res = new BitBigInt (divided);
			res.Mod (division);
			return res;
		}

		public void Mod (uint integer) {
			if (integer == Constants.LEVEL_IS_EMPTY) {
				throw new Exceptions.DivisionByZero(this);
				return;
			}
			this.Mod (new BitBigInt (false, integer));
		}

		public BitBigInt Mod_New (uint integer) {
			BitBigInt res = new BitBigInt (this);
			res.Mod (integer);
			return res;
		}

		// LOGARITHM

		public void Log (uint base,  bool isSigned) {

			if (this.Check_IsOne) {
				self.Set_Zero();
				return;
			}
			if (this.CompareUnsigned (base) == -1) {
				return;
			}
			if (this.Check_IsZero() || base == 0) {
				return;
			}

			uint curPower = 1;
			BitBigInt compare = new BitBigInt (isSigned, base);
			while (this.CompareUnsigned (compare) == 1) {
				compare.Set_SystemInteger (isSigned, base);
				compare.Pow (++curPower);
			}
			this.Set_SystemInteger ((isSigned == true && curPower % 2 != 0) ? true : false, curPower);
		}

		public BitBigInt Log_New (uint base,  bool isSigned) {

			if (this.Check_IsOne) {
				return new BitBigInt();
			}
			if (this.CompareUnsigned (base) == -1) {
				return null;
			}
			if (this.Check_IsZero() || base == 0) {
				return null;
			}

			uint curPower = 1;
			BitBigInt compare = new BitBigInt (isSigned, base);
			while (this.CompareUnsigned (compare) == 1) {
				compare.Set_SystemInteger (isSigned, base);
				compare.Pow (++curPower);
			}
			compare.Set_SystemInteger ((isSigned == true && curPower % 2 != 0) ? true : false, curPower);
			return compare;
		}

		public void Log (BitBigInt base) {

			if (this.Check_IsOne) {
				self.Set_Zero();
				return;
			}
			if (this.CompareUnsigned (base) == -1) {
				return;
			}
			if (this.Check_IsZero() || base.Check_IsZero()) {
				return;
			}

			uint curPower = 1;
			BitBigInt compare = new BitBigInt(base);

			while (this.CompareUnsigned (compare) == 1) {
				compare.Copy (base);
				compare.Pow (++curPower);
			}
			this.Set_SystemInteger ((base.sign == true && curPower % 2 != 0) ? true : false, curPower);
		}

		public BitBigInt Log_New (BitBigInt base) {

			if (this.Check_IsOne) {
				return new BitBigInt();
			}
			if (this.CompareUnsigned (base) == -1) {
				return;
			}
			if (this.Check_IsZero() || base.Check_IsZero()) {
				return;
			}

			uint curPower = 1;
			BitBigInt compare = new BitBigInt(base);

			while (this.CompareUnsigned (compare) == 1) {
				compare.Copy (base);
				compare.Pow (++curPower);
			}
			compare.Set_SystemInteger ((base.sign == true && curPower % 2 != 0) ? true : false, curPower);
			return compare;
		}

		// SQRT

		public void Sqrt () {
			BitBigInt compare = new BitBigInt(this);
			BitBigInt temp = this.Pow_New(2);

			while (compare.CompareUnsigned(temp) == -1) {
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

		public BitBigInt Sqrt_New () {
			BitBigInt compare = new BitBigInt(this);
			BitBigInt temp = compare.Pow_New(2);

			while (this.CompareUnsigned(temp) == -1) {
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

		// @PRINT


		// @BASE

	}

	// testing
	class Program {

    	static void Main(string[] args)
    		{

    		}
	}

}
