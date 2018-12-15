﻿using System;

namespace bbi {

	public class CBBI {

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

	public class EBBI {

		[Serializable]
		public class ExceptionBBI : Exception {
			public ExceptionBBI () {}
			public ExceptionBBI (string type)
			: base (String.Format("BitBigInt Exception: {0}", type)){}
		}

		[Serializable]
		public class InvalidBitInBitflag : ExceptionBBI {
			public InvalidBitInBitflag () {}
			public InvalidBitInBitflag (short bit)
			: base(String.Format("Invalid Bit In Bitflag {0} ; Must be from 1 to {1}", bit, CBBI.INTEGER_SIZE)){}
		}

	}

	// stuaa, but C# have unsigned int
	public class uintArithmetics {

		public uint Bitflag (short bit) {

			if ( !(bit >= 0 && bit <= CBBI.INTEGER_SIZE) )
				throw new EBBI.InvalidBitInBitflag(bit);

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
			for (uint curBit = 2; curBit <= CBBI.INTEGER_SIZE; curBit++)
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
			this.at = new uint[CBBI.LEVEL_COUNT];
			this.sign = false;
			this.lvlButton = CBBI.LEVEL_TOP;
		}

		// from system integer
		public BitBigInt (bool sign, uint integer) {
			this.at = new uint[CBBI.LEVEL_COUNT];
			this.sign = sign;
			this.lvlButton = CBBI.LEVEL_TOP;
			this.at[CBBI.LEVEL_TOP] = integer;
		}
		// copy new
		public BitBigInt (BitBigInt copyFrom) {
			this.at = new uint[CBBI.LEVEL_COUNT];
			this.Copy (copyFrom);
		}

		public void Sign_Change () => this.sign = (this.sign) ? false : true;
		public void Sign_Set (bool newSign) => this.sign = newSign;

		public void LvlButton_Configure () {
			for (uint curLvl = 0; curLvl < CBBI.LEVEL_TOP; curLvl++)
				if (this.at[curLvl] != CBBI.LEVEL_IS_EMPTY) {
					this.lvlButton = curLvl;
					return;
				}
			this.lvlButton = CBBI.LEVEL_TOP;
		}
		public void LvlButton_Set (uint level) => this.lvlButton = level;

		private void _setters_op (uint val, bool sign) {
			if (val != CBBI.LEVEL_IS_EMPTY) {
				for (uint curLvl = 0; curLvl <= CBBI.LEVEL_TOP; curLvl++) this.at[curLvl] = val;
				this.sign = sign;
				this.lvlButton = 0;
			}
			else {
				for (uint curLvl = 0; curLvl <= CBBI.LEVEL_TOP; curLvl++) this.at[curLvl] = CBBI.LEVEL_IS_EMPTY;
				this.sign = false;
				this.lvlButton = CBBI.LEVEL_TOP;
			}
		}
		public void Set_Zero () => this._setters_op (CBBI.LEVEL_IS_EMPTY, false);
		public void Set_Min () => this._setters_op (CBBI.LEVEL_IS_FULL, true);
		public void Set_Max () => this._setters_op (CBBI.LEVEL_IS_FULL, false);
		public void Set_Value (uint value) => this._setters_op (value, false);
		public void Set_Value_Signed (uint value) => this._setters_op (value, true);

		public void Set_Value_FromLevel (uint level, uint value) {
			for (uint curLvl = 0; curLvl <= level; curLvl++)
				this.at[curLvl] = value;
			this.sign = false;
			this.lvlButton = 0; if (value == CBBI.LEVEL_IS_EMPTY) bbia_lvlButton_conf (self);
		}

		public void Set_Value_FromLevel_Signed (uint level, uint value) {
			this.Set_Value_FromLevel (level,value);
			this.sign = true;
		}

		public void Set_Value_ToLevel (uint level, uint value) {
			for (uint curLvl = level; curLvl <= CBBI.LEVEL_TOP; curLvl++)
				this.at[curLvl] = value;
			this.sign = false;
			this.lvlButton = level; if (value == CBBI.LEVEL_IS_EMPTY) bbia_lvlButton_conf (self);
		}

		public void Set_Value_ToLevel_Signed (uint level, uint value) {
			this.Set_Value_ToLevel (level,value);
			this.sign = true;
		}

		public void Set_SystemInteger (bool isSigned, uint integer) {
			this.Set_Zero();
			this.at[CBBI.LEVEL_TOP] = integer;
			this.sign = isSigned;
		}


		public bool Check_IsInteger (uint integer) {
			if (this.lvlButton < CBBI.LEVEL_TOP) return false;
			return (this.at[CBBI.LEVEL_TOP] == integer) ? true : false;
		}
		public bool Check_IsZero () => return this.Check_IsInteger (0);
		public bool Check_IsOne () => return this.Check_IsInteger (1);
		public bool Check_IsSystemInteger () => return (this.lvlButton == CBBI.LEVEL_TOP) ? true : false;

		public int Compare_Unsigned (BitBigInt b) {
			int curCompare = 0;
			int curLvl = (this.lvlButton <= b.lvlButton) ? this.lvlButton : b.lvlButton;
			for (; curLvl <= CBBI.LEVEL_TOP; curLvl++) {
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
			if (this.lvlButton < CBBI.LEVEL_TOP) return 1;
			return uintArithmetics.Compare (this.at[CBBI.LEVEL_TOP], toCompare);
		}

		public int Compare (bool isSigned, uint toCompare) {
			int res = bbia_compare_int_unsigned (self, toCompare);

			if (this.sign == isSigned) return res;
			else if (this.sign == true) return -1;
			else if (isSigned == true) return 1;
			return 0;
		}

		public void Copy (BitBigInt from) {
			for (uint curLvl = 0; curLvl <= CBBI.LEVEL_TOP; curLvl++)
				this.at[curLvl] = from.at[curLvl];
			this.sign = from.sign;
			this.lvlButton = from.lvlButton;
		}

		// @BIT_OPERATIONS

		void Bits_Shift_Left (uint value) {

			uint savedBits [CBBI.LEVEL_TOP];
			uint bitMask[2] = {0,0};
			for (uint curBit = CBBI.INTEGER_SIZE-value+1; curBit <= CBBI.INTEGER_SIZE; curBit++)
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
			for (uint lvl = CBBI.LEVEL_TOP; lvl > this.lvlButton; this.at[lvl] <<= value, lvl--) {
				bitMask[1] = this.at[lvl] & bitMask[0];
				bitMask[1] >>= CBBI.INTEGER_SIZE-value;
				savedBits[lvl-1] = bitMask[1];
			}
			this.at[this.lvlButton] <<= value;

			for (uint lvl = this.lvlButton; lvl < CBBI.LEVEL_TOP; lvl++)
				this.at[lvl] |= savedBits[lvl];
		}

		void Bits_Shift_Right (uint value) {

			uint savedBits [CBBI.LEVEL_TOP];
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
			for (uint lvl = this.lvlButton; lvl < CBBI.LEVEL_TOP; this.at[lvl] >>= value, lvl++) {
				bitMask[1] = this.at[lvl] & bitMask[0];
				bitMask[1] <<= CBBI.INTEGER_SIZE-value;
				savedBits[lvl] = bitMask[1];
			}
			this.at[CBBI.LEVEL_TOP] >>= value;

			for (uint lvl = this.lvlButton; lvl < CBBI.LEVEL_TOP; lvl++)
				this.at[lvl+1] |= savedBits[lvl];
			if (this.at[self->lvlButton] == CBBI.LEVEL_IS_EMPTY) this.lvlButton++;
		}

		void bbia_bits_or (bbia * first, bbia * second) {
			nullPointer_funcVoid_2 (first,second,"bbia_bits_or");
			first->lvlButton = (stuaa_compare (first->lvlButton, second->lvlButton) != -1) ? first->lvlButton : second->lvlButton;

			for (int curLvl = first->lvlButton; stuaa_compare (curLvl, CBBI.LEVEL_TOP) != 1; curLvl++)
				first->at[curLvl] |= second->at[curLvl];
		}

		bbia * bbia_bits_or_new (bbia * first, bbia * second) {
			nullPointer_funcPointer_2 (first,second,"bbia_bits_or_new");
			bbia * self = bbia_new();
			self->lvlButton = (stuaa_compare (first->lvlButton, second->lvlButton) != -1) ? first->lvlButton : second->lvlButton;

			for (int curLvl = self->lvlButton; stuaa_compare (curLvl, CBBI.LEVEL_TOP) != 1; curLvl++)
				self->at[curLvl] = first->at[curLvl] | second->at[curLvl];
			return self;
		}

		void bbia_bits_and (bbia * first, bbia * second) {
			nullPointer_funcVoid_2 (first,second,"bbia_bits_and");
			first->lvlButton = (stuaa_compare (first->lvlButton, second->lvlButton) != -1) ? first->lvlButton : second->lvlButton;

			for (int curLvl = first->lvlButton; stuaa_compare (curLvl, CBBI.LEVEL_TOP) != 1; curLvl++)
				first->at[curLvl] &= second->at[curLvl];
		}

		bbia * bbia_bits_and_new (bbia * first, bbia * second) {
			nullPointer_funcPointer_2 (first,second,"bbia_bits_and_new");
			bbia * self = bbia_new();
			self->lvlButton = (stuaa_compare (first->lvlButton, second->lvlButton) != -1) ? first->lvlButton : second->lvlButton;

			for (int curLvl = self->lvlButton; stuaa_compare (curLvl, CBBI.LEVEL_TOP) != 1; curLvl++)
				self->at[curLvl] = first->at[curLvl] & second->at[curLvl];
			return self;
		}

		void bbia_bits_flag_set (bbia * self, int num) {
			nullPointer_funcVoid_1 (self,"bbia_bits_flag_set");
			int lvl = CBBI.LEVEL_TOP - num / CBBI.INTEGER_SIZE;
			num %= CBBI.INTEGER_SIZE;
			if (num != 0)
				self->at[lvl] |= uintArithmetics.Bitflag (num);
			else
				self->at[lvl+1] |= uintArithmetics.Bitflag (CBBI.INTEGER_SIZE);
			bbia_lvlButton_conf (self);
		}

		void bbia_bits_flag_unset (bbia * self, int num) {
			nullPointer_funcVoid_1 (self,"bbia_bits_flag_unset");
			int lvl = CBBI.LEVEL_TOP - num / CBBI.INTEGER_SIZE;
			num %= CBBI.INTEGER_SIZE;
			if (num != 0)
				self->at[lvl] &= ~uintArithmetics.Bitflag (num);
			else
				self->at[lvl+1] &= ~uintArithmetics.Bitflag (CBBI.INTEGER_SIZE);
			bbia_lvlButton_conf (self);
		}

		void bbia_bits_flag_set_mult (bbia * self, int * numArray) {
			nullPointer_funcVoid_2 (self,numArray,"bbia_bits_flag_set_mult");
			while (numArray != NULL)
				bbia_bits_flag_set (self, *numArray++);
		}

		void bbia_bits_flag_unset_mult (bbia * self, int * numArray) {
			nullPointer_funcVoid_2 (self,numArray,"bbia_bits_flag_unset_mult");
			while (numArray != NULL)
				bbia_bits_flag_unset (self, *numArray++);
		}

		bbia * bbia_bits_flag (int num) {
			bbia * self = bbia_new ();
			int lvl = CBBI.LEVEL_TOP - num / CBBI.INTEGER_SIZE;
			num %= CBBI.INTEGER_SIZE;
			if (num != 0)
				self->at[lvl] |= uintArithmetics.Bitflag (num);
			else
				self->at[lvl+1] |= uintArithmetics.Bitflag (CBBI.INTEGER_SIZE);

			bbia_lvlButton_conf (self);
			return self;
		}

		bbia * bbia_bits_isFull () {
			bbia * self = bbia_new();
			for (int curLvl = 0; stuaa_compare (curLvl, CBBI.LEVEL_TOP) != 1; curLvl++)
				self->at[curLvl] = CBBI.LEVEL_IS_FULL;
			self->lvlButton = 0;
			return self;
		}

		bbia * bbia_bits_isEmpty () {
			bbia * self = bbia_new();
			for (int curLvl = 0; stuaa_compare (curLvl, CBBI.LEVEL_TOP) != 1; curLvl++)
				self->at[curLvl] = CBBI.LEVEL_IS_EMPTY;
			self->lvlButton = CBBI.LEVEL_TOP;
			return self;
		}

		bbia * bbia_bits_tillBit_isFull (int num) {
			bbia * self = bbia_new();
			int lvl = CBBI.LEVEL_TOP - num / CBBI.INTEGER_SIZE;
			num %= CBBI.INTEGER_SIZE;
			if (num == 0) lvl++;

			for (int curLvl = lvl + 1; stuaa_compare (curLvl, CBBI.LEVEL_TOP) != 1; curLvl--)
				self->at[curLvl] = CBBI.LEVEL_IS_FULL;
			for (int curBit = 1; stuaa_compare (curBit, num) != 1; curBit++)
				self->at[lvl] |= uintArithmetics.Bitflag (curBit);
			bbia_lvlButton_conf (self);
			return self;
		}

		bbia * bbia_bits_tillBit_isEmpty (int num) {
			bbia * self = bbia_new();
			int lvl = CBBI.LEVEL_TOP - num / CBBI.INTEGER_SIZE;
			num %= CBBI.INTEGER_SIZE;
			if (num == 0) lvl++;

			for (int curLvl = lvl + 1; stuaa_compare (curLvl, CBBI.LEVEL_TOP) != 1; curLvl--)
				self->at[curLvl] = CBBI.LEVEL_IS_EMPTY;
			for (int curBit = 1; stuaa_compare (curBit, num) != 1; curBit++)
				self->at[lvl] &= ~(uintArithmetics.Bitflag (curBit));
			bbia_lvlButton_conf (self);
			return self;
		}

	}

	// testing
	class Program {

    	static void Main(string[] args)
    		{

    		}
	}

}
