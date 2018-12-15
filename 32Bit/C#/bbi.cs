﻿using System;

namespace bbi {

	// Big Big Integer Constants
	public class BBIC {

		public const uint INTEGER_SIZE = 32;

		/*SIZE*/
		public const uint BITS_512 = 512/INTEGER_SIZE ;// 2^9
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
	public class BBIE {

		[Serializable]
		public class BBIException : Exception {
			public BBIException () {}
			public BBIException (string type)
			: base (String.Format("BitBigInt Exception: {0}", type)){}
		}

		[Serializable]
		public class InvalidBitInBitflag : BBIException {
			public InvalidBitInBitflag () {}
			public InvalidBitInBitflag (short bit)
			: base(String.Format("Invalid Bit In Bitflag {0} ; Must be from 1 to {1}", bit, BBIC.INTEGER_SIZE)){}
		}

	}

	// stuaa, but C# have unsigned int
	public class uintArithmetics {

		public uint Bitflag (short bit) {

			if ( !(bit >= 0 && bit <= BBIC.INTEGER_SIZE) )
				throw new BBIE.InvalidBitInBitflag(bit);

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
			for (uint curBit = 2; curBit <= BBIC.INTEGER_SIZE; curBit++)
				if (value == Bitflag (curBit))
					return true;
			return false;
		}

		public int Comapre (uint a, uint b) => return (a == b) ? 0 : (a > b) ? 1 : -1;
	}

	public class BitBigInt {

		// FIELDS
		public uint[] at;
		public bool sign;
		public uint lvlButton;

		public BitBigInt () {
			this.at = new uint[BBIC.LEVEL_COUNT];
			this.sign = false;
			this.lvlButton = BBIC.LEVEL_TOP;
		}

		// from system integer
		public BitBigInt (bool sign, uint integer) {
			this.at = new uint[BBIC.LEVEL_COUNT];
			this.sign = sign;
			this.lvlButton = BBIC.LEVEL_TOP;
			this.at[BBIC.LEVEL_TOP] = integer;
		}
		// copy new
		public BitBigInt (BitBigInt copyFrom) {
			this.at = new uint[BBIC.LEVEL_COUNT];
			this.Copy (copyFrom);
		}

		public void Sign_Change () => this.sign = (this.sign) ? false : true;
		public void Sign_Set (bool newSign) => this.sign = newSign;

		public void LvlButton_Configure () {
			for (uint curLvl = 0; curLvl < BBIC.LEVEL_TOP; curLvl++)
				if (this.at[curLvl] != BBIC.LEVEL_IS_EMPTY) {
					this.lvlButton = curLvl;
					return;
				}
			this.lvlButton = BBIC.LEVEL_TOP;
		}
		public void LvlButton_Set (uint level) => this.lvlButton = level;

		private void _setters_op (uint val, bool sign) {
			if (val != BBIC.LEVEL_IS_EMPTY) {
				for (uint curLvl = 0; curLvl <= BBIC.LEVEL_TOP; curLvl++) this.at[curLvl] = val;
				this.sign = sign;
				this.lvlButton = 0;
			}
			else {
				for (uint curLvl = 0; curLvl <= BBIC.LEVEL_TOP; curLvl++) this.at[curLvl] = BBIC.LEVEL_IS_EMPTY;
				this.sign = false;
				this.lvlButton = BBIC.LEVEL_TOP;
			}
		}
		public void Set_Zero () => this._setters_op (BBIC.LEVEL_IS_EMPTY, false);
		public void Set_Min () => this._setters_op (BBIC.LEVEL_IS_FULL, true);
		public void Set_Max () => this._setters_op (BBIC.LEVEL_IS_FULL, false);
		public void Set_Value (uint value) => this._setters_op (value, false);
		public void Set_Value_Signed (uint value) => this._setters_op (value, true);

		public void Set_Value_FromLevel (uint level, uint value) {
			for (uint curLvl = 0; curLvl <= level; curLvl++)
				this.at[curLvl] = value;
			this.sign = false;
			this.lvlButton = 0; if (value == BBIC.LEVEL_IS_EMPTY) this.LvlButton_Configure();
		}

		public void Set_Value_FromLevel_Signed (uint level, uint value) {
			this.Set_Value_FromLevel (level,value);
			this.sign = true;
		}

		public void Set_Value_ToLevel (uint level, uint value) {
			for (uint curLvl = level; curLvl <= BBIC.LEVEL_TOP; curLvl++)
				this.at[curLvl] = value;
			this.sign = false;
			this.lvlButton = level; if (value == BBIC.LEVEL_IS_EMPTY) this.LvlButton_Configure();
		}

		public void Set_Value_ToLevel_Signed (uint level, uint value) {
			this.Set_Value_ToLevel (level,value);
			this.sign = true;
		}

		public void Set_SystemInteger (bool isSigned, uint integer) {
			this.Set_Zero();
			this.at[BBIC.LEVEL_TOP] = integer;
			this.sign = isSigned;
		}


		public bool Check_IsInteger (uint integer) {
			if (this.lvlButton < BBIC.LEVEL_TOP) return false;
			return (this.at[BBIC.LEVEL_TOP] == integer) ? true : false;
		}
		public bool Check_IsZero () => return this.Check_IsInteger (0);
		public bool Check_IsOne () => return this.Check_IsInteger (1);
		public bool Check_IsSystemInteger () => return (this.lvlButton == BBIC.LEVEL_TOP) ? true : false;

		public int Compare_Unsigned (BitBigInt b) {
			int curCompare = 0;
			int curLvl = (this.lvlButton <= b.lvlButton) ? this.lvlButton : b.lvlButton;
			for (; curLvl <= BBIC.LEVEL_TOP; curLvl++) {
				curCompare = uintArithmetics.Compare (this.at[curLvl], b.at[curLvl]);
				if (curCompare != 0)
					return curCompare;
			}
			return 0;
		}

		public int Compare (BitBigInt b) {
			int res = this.Compare (b);

			if (this.sign == b.sign) return res;
			else if (this.sign == true) return -1;
			else if (b.sign == true) return 1;
			return 0;
		}

		public int Compare_Unsigned (uint toCompare) {
			if (this.lvlButton < BBIC.LEVEL_TOP) return 1;
			return uintArithmetics.Compare (this.at[BBIC.LEVEL_TOP], toCompare);
		}

		public int Compare (bool isSigned, uint toCompare) {
			int res = bbia_compare_int_unsigned (self, toCompare);

			if (this.sign == isSigned) return res;
			else if (this.sign == true) return -1;
			else if (isSigned == true) return 1;
			return 0;
		}

		public void Copy (BitBigInt from) {
			for (uint curLvl = 0; curLvl <= BBIC.LEVEL_TOP; curLvl++)
				this.at[curLvl] = from.at[curLvl];
			this.sign = from.sign;
			this.lvlButton = from.lvlButton;
		}

		// @BIT_OPERATIONS

		public void Bits_Shift_Left (uint value) {

			uint savedBits [BBIC.LEVEL_TOP];
			uint bitMask[2] = {0,0};
			for (uint curBit = BBIC.INTEGER_SIZE-value+1; curBit <= BBIC.INTEGER_SIZE; curBit++)
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
			for (uint lvl = BBIC.LEVEL_TOP; lvl > this.lvlButton; this.at[lvl] <<= value, lvl--) {
				bitMask[1] = this.at[lvl] & bitMask[0];
				bitMask[1] >>= BBIC.INTEGER_SIZE-value;
				savedBits[lvl-1] = bitMask[1];
			}
			this.at[this.lvlButton] <<= value;

			for (uint lvl = this.lvlButton; lvl < BBIC.LEVEL_TOP; lvl++)
				this.at[lvl] |= savedBits[lvl];
		}

		public void Bits_Shift_Right (uint value) {

			uint savedBits [BBIC.LEVEL_TOP];
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
			for (uint lvl = this.lvlButton; lvl < BBIC.LEVEL_TOP; this.at[lvl] >>= value, lvl++) {
				bitMask[1] = this.at[lvl] & bitMask[0];
				bitMask[1] <<= BBIC.INTEGER_SIZE-value;
				savedBits[lvl] = bitMask[1];
			}
			this.at[BBIC.LEVEL_TOP] >>= value;

			for (uint lvl = this.lvlButton; lvl < BBIC.LEVEL_TOP; lvl++)
				this.at[lvl+1] |= savedBits[lvl];
			if (this.at[this.lvlButton] == BBIC.LEVEL_IS_EMPTY) this.lvlButton++;
		}

		public void Bits_Or (BitBigInt second) {
			this.lvlButton = (this.lvlButton <= second.lvlButton) ? this.lvlButton : second.lvlButton;
			for (int curLvl = this.lvlButton; curLvl <= BBIC.LEVEL_TOP; curLvl++)
				this.at[curLvl] |= second.at[curLvl];
		}

		public BitBigInt Bits_Or_New (BitBigInt second) {
			BitBigInt result = new BitBigInt(this);
			result.Bits_Or(second);
			return result;
		}

		public void Bits_And (BitBigInt second) {
			this.lvlButton = (this.lvlButton <= second.lvlButton) ? this.lvlButton : second.lvlButton;
			for (int curLvl = this.lvlButton; curLvl <= BBIC.LEVEL_TOP; curLvl++)
				this.at[curLvl] &= second.at[curLvl];
		}

		public BitBigInt Bits_And_New (BitBigInt second) {
			BitBigInt result = new BitBigInt(this);
			result.Bits_And(second);
			return result;
		}

		public void Bits_Flag_Set (uint bit) {
			uint lvl = BBIC.LEVEL_TOP - bit / BBIC.INTEGER_SIZE;
			bit %= BBIC.INTEGER_SIZE;
			if (bit != 0)
				self.at[lvl] |= uintArithmetics.Bitflag (bit);
			else
				self.at[lvl+1] |= uintArithmetics.Bitflag (BBIC.INTEGER_SIZE);
			this.LvlButton_Configure();
		}

		public void Bits_Flag_Unset (uint bit) {
			uint lvl = BBIC.LEVEL_TOP - bit / BBIC.INTEGER_SIZE;
			bit %= BBIC.INTEGER_SIZE;
			if (bit != 0)
				self.at[lvl] &= ~uintArithmetics.Bitflag (bit);
			else
				self.at[lvl+1] &= ~uintArithmetics.Bitflag (BBIC.INTEGER_SIZE);
			this.LvlButton_Configure();
		}

		public void Bits_Flag_Set_Mult (uint[] bitFlagArray) {
			for (uint curFlag = 0; curFlag < bitFlagArray.length; curFlag++)
				this.Bits_Flag_Set (bitFlagArray[curFlag]);
		}

		public void Bits_Flag_Unset_Mult (uint[] bitFlagArray) {
			for (uint curFlag = 0; curFlag < bitFlagArray.length; curFlag++)
				this.Bits_Flag_Unset (bitFlagArray[curFlag]);
		}

		public BitBigInt Bits_Flag_New (uint bit) {
			BitBigInt result = new BitBigInt();
			uint lvl = BBIC.LEVEL_TOP - bit / BBIC.INTEGER_SIZE;
			bit %= BBIC.INTEGER_SIZE;
			if (bit != 0)
				result.at[lvl] |= uintArithmetics.Bitflag (bit);
			else
				result.at[lvl+1] |= uintArithmetics.Bitflag (BBIC.INTEGER_SIZE);

			result.LvlButton_Configure();
			return result;
		}

		public BitBigInt Bits_Full_New () {
			BitBigInt result = new BitBigInt();
			for (uint curLvl = 0; curLvl <= BBIC.LEVEL_TOP; curLvl++)
				result.at[curLvl] = BBIC.LEVEL_IS_FULL;
			result.lvlButton = 0;
			return result;
		}
		public BitBigInt Bits_Empty_New () => return new BitBigInt();

		public BitBigInt Bits_Full_TillBit_New (int bit) {
			BitBigInt result = bbia_new();
			uint lvl = BBIC.LEVEL_TOP - bit / BBIC.INTEGER_SIZE;
			bit %= BBIC.INTEGER_SIZE;
			if (bit == 0) lvl++;

			for (uint curLvl = lvl + 1; curLvl < BBIC.LEVEL_TOP; curLvl--)
				result.at[curLvl] = BBIC.LEVEL_IS_FULL;
			for (uint curBit = 1; curBit < bit; curBit++)
				result.at[lvl] |= uintArithmetics.Bitflag (curBit);
			this.LvlButton_Configure();
			return result;
		}

		public BitBigInt Bits_Full_TillBit_New (int bit) {
			BitBigInt result = bbia_new();
			uint lvl = BBIC.LEVEL_TOP - bit / BBIC.INTEGER_SIZE;
			bit %= BBIC.INTEGER_SIZE;
			if (bit == 0) lvl++;

			for (uint curLvl = lvl + 1; curLvl < BBIC.LEVEL_TOP; curLvl--)
				result.at[curLvl] = BBIC.LEVEL_IS_EMPTY;
			for (uint curBit = 1; curBit < bit; curBit++)
				result.at[lvl] &= ~(uintArithmetics.Bitflag (curBit));
			this.LvlButton_Configure();
			return result;
		}

		// ADD/SUB private module operations

		internal protected void _add_bbi_op (BitBigInt from) {
			// if from is system integer and out of bounders not possible then try simple language addition
			if (from.Check_IsSystemInteger() && this.at[0] < BBIC.LEVEL_IS_FULL) {
				this.Add_Level (from.at[BBIC.LEVEL_TOP], BBIC.LEVEL_TOP);
				return;
			}
			else {
				this.lvlButton = (this.lvlButton <= from.lvlButton) ? this.lvlButton : from.lvlButton;
				uint level = BBIC.LEVEL_TOP;
				register short outBit = 0;
				register short curBit = 0;

				// standart addition algorithm for binary N-bit integer
				for (; (level >= this.lvlButton) || (outBit != 0 && level > 0); level--)
				for (uint bitPos = 1; bitPos <= BBIC.INTEGER_SIZE; bitPos++) {
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
					level = BBIC.LEVEL_TOP;
					for (; outBit != 0 && level > 0; level--)
					for (uint bitPos = 1; bitPos <= BBIC.INTEGER_SIZE; outBit--, bitPos++)
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
			&& uintArithmetics.OutOfUint_Sub (this.at[BBIC.LEVEL_TOP], subtrahend.at[BBIC.LEVEL_TOP]) == false) {
				this.Sub_Level (subtrahend.at[BBIC.LEVEL_TOP], BBIC.LEVEL_TOP);
				return;
			}
			// OUT OF BOUNDERS
			else if (this.Compare_Unsigned (subtrahend) == -1) {
				BitBigInt temp = new BitBigInt(subtrahend);
				// @TODO private method use
				temp._sub_bbi_op (this);
				temp.Sign_Set ((this.sign == false) ? true : false);
				this.Copy(temp);
				return;
			}
			else {
				uint bitPos = 1;
				uint level = BBIC.LEVEL_TOP;
				register short outBit = 0;
				register short curBitSecond = 0;
				register short curBitFirst = 0;

				for (; level >= this.lvlButton; level--)
				for (bitPos = 1; bitPos <= BBIC.INTEGER_SIZE; bitPos++) {
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
				this.at[curLvl] = BBIC.LEVEL_IS_EMPTY;
			// x = x + y
			// the number needed to overflow is z = FULL - y + 1
			// the value to set is x = EMPTY + x - z
			this.at[fromLvl] = BBIC.LEVEL_IS_EMPTY + this.at[fromLvl] - (BBIC.LEVEL_IS_FULL - integer + 1);
		}

		public void Add_Level (uint integer, uint level) {
			if (uintArithmetics.OutOfUint_Add (this.at[level], integer) == false)
				this.at[level] += integer;
			else
				this.Add_LevelOut (integer, level, level);
		}

		public void Add (uint integer) {
			if (this.sign == false)
				this.Add_Level (integer, BBIC.LEVEL_TOP);
			else if (this.sign == true)
				this.Sub_Level (integer, BBIC.LEVEL_TOP);
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
				this.at[curLvl] = BBIC.LEVEL_IS_FULL;
			// x = x - y
			// the number needed to overflow is z = EMPTY + y - 1
			// the value to set x = FULL - x + z
			this.at[fromLvl] = BBIC.LEVEL_IS_FULL - this.at[fromLvl] + (BBIC.LEVEL_IS_EMPTY + integer - 1);

			for (int curLvl = prevLvl+1; curLvl <= BBIC.LEVEL_TOP; curLvl++)
				this.at[curLvl] = BBIC.LEVEL_IS_FULL - this.at[curLvl] + 1;
		}

		public void Sub_Level (uint integer, uint level) {
			if (uintArithmetics.OutOfUint_Sub (this.at[level], integer) == false)
				this.at[level] -= integer;
			else
				this.Sub_LevelOut (integer, level, level);
		}

		public void Sub (uint integer) {
			if (this.sign == false)
				this.Sub_Level (integer, BBIC.LEVEL_TOP);
			else if (this.sign == true)
				this.Add_Level (integer, BBIC.LEVEL_TOP);
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

	}

	// testing
	class Program {

    	static void Main(string[] args)
    		{

    		}
	}

}
