public class bbia {

	public final int BBIA_INTEGER_SIZE = 32;

	/*SIZE*/
	public final int BBIA_BITS_512 =  512/BBIA_INTEGER_SIZE; // 2^9
	public final int BBIA_BITS_1024 = 1024/BBIA_INTEGER_SIZE; // 2^10
	public final int BBIA_BITS_2048 = 2048/BBIA_INTEGER_SIZE; // 2^11
	public final int BBIA_BITS_4096 = 4096/BBIA_INTEGER_SIZE; // 2^12
	public final int BBIA_BITS_8192 = 8192/BBIA_INTEGER_SIZE; // 2^13
	public final int BBIA_BITS_16384 = 16384/BBIA_INTEGER_SIZE; // 2^14
	public final int BBIA_BITS_32768 = 32768/BBIA_INTEGER_SIZE; // 2^15
	public final int BBIA_BITS_65536 = 65536/BBIA_INTEGER_SIZE; // 2^16
	public final int BBIA_BITS_131072 = 131072/BBIA_INTEGER_SIZE; // 2^17
	public final int BBIA_BITS_262144 = 262144/BBIA_INTEGER_SIZE; // 2^18
	public final int BBIA_BITS_524288 = 524288/BBIA_INTEGER_SIZE; // 2^19
	public final int BBIA_BITS_1048576 = 1048576/BBIA_INTEGER_SIZE; // 2^20


	/*DIGIT*/
	// signed int 1111...n (n == BBIA_INTEGER_SIZE)
	public final int BBIA_LEVEL_IS_FULL = -1 ;
	// signed int 0000...n (n == BBIA_INTEGER_SIZE)
	public final int BBIA_LEVEL_IS_EMPTY = 0 ;

	/*SIGN*/
	public final bool BBIA_SIGNED = true ;
	public final bool BBIA_UNSIGNED = false ;

	// @STUAA
	private class stuaa {

		final char[65] numerics =
		"0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

		int intPow (int value, int pow) {
			if (pow != 1)
				return value * intPow(value, pow - 1);
			else return value;
		}

		double log_base(double base, double num) {
			return log2(num) / log2(base);
		}

		int findDigitInNumerics (char[] numicsStr, char digit) {
			for (int curNumicsInd = 0; curNumicsInd < BBIA_INTEGER_SIZE;
				curNumicsInd++)

				if (numicsStr [curNumicsInd] == digit)
					return curNumicsInd;

			return -1;
		}

		int bitflag (int num) {

			if ( !(num >= 0 && num <= BBIA_INTEGER_SIZE) )
				return 0;

			if (BBIA_INTEGER_SIZE < 32 || num < 32) {

				final int[33] bitDigit = { 0,
					0x1, 0x2, 0x4, 0x8,
					0x10, 0x20, 0x40, 0x80,
					0x100, 0x200, 0x400, 0x800,
					0x1000, 0x2000, 0x4000, 0x8000,
					0x10000, 0x20000, 0x40000, 0x80000,
					0x100000, 0x200000, 0x400000, 0x800000,
					0x1000000, 0x2000000, 0x4000000, 0x8000000,
					0x10000000, 0x20000000, 0x40000000, 0x80000000
				};

				return bitDigit[num];
			}

			int bitDigit = 0x40000000;

			while (num-- != 31)
				bitDigit <<= 1;

			return bitDigit;
		}

		int shiftr (int self, int value) {
			int signedDebug = 0;

			if (bitflag(BBIA_INTEGER_SIZE) & self) {
				signedDebug = 1;
				self &= ~bitflag(BBIA_INTEGER_SIZE);
			}

			self >>= value;

			if (signedDebug) {
				signedDebug = 0;
				self |= bitflag(BBIA_INTEGER_SIZE-value);
			}

			return self;
		}

		char[] toBase (int sinteger, int base) {

			if ( !(base < 65 && base > 1) ) {
				// throw ("The base must be from 2 to 64");
				return null;
			}

			char[] result = new char[BBIA_INTEGER_SIZE + 1];

			for (int currentBit = 1; currentBit <= BBIA_INTEGER_SIZE;

				result[BBIA_INTEGER_SIZE-currentBit] =
				(sinteger & bitflag(currentBit)) ? '1' : '0',
				currentBit++
			);

			if (base == 2) 	return result;

			return toBase_from2Base (result, base);
		}

		int from2Base (char[] integer) {

			int result = 0;
			for (int currentBit = 1; currentBit <= BBIA_INTEGER_SIZE;

				result |= (integer[BBIA_INTEGER_SIZE-currentBit] == '1')
				? stuaa_bitflag(currentBit) : 0,
				currentBit++
			);

			return result;
		}

		char[] toBase_from2Base (char[] buffer, int base) {

			int inDec = from2Base (buffer);
			// @TODO STUAA

			return buffer;
		}

		int fromBase (char[] integer, int base) {

			// @TODO STUAA

			return -1;
		}
	}

	// @BBIA

	private int[] at;
	private bool sign;

	// default size of bbia
	private int BBIA_LEVEL_COUNT = BBIA_BITS_512 ;
	private int BBIA_LEVEL_TOP = -1 + BBIA_LEVEL_COUNT ;

	// @Constructor
	public bbia () {
		this.at = new int[BBIA_LEVEL_COUNT];
		this.sign = BBIA_UNSIGNED;
	}

	// @Constructor for custom sign
	public bbia (bool sign) {
		this.at = new int[BBIA_LEVEL_COUNT];
		this.sign = sign;
	}

	// @Constructor for custom bitsize
	public bbia (int bits) {
		this.BBIA_LEVEL_COUNT = bits / BBIA_INTEGER_SIZE;
		if (bits % BBIA_INTEGER_SIZE != 0) this.BBIA_LEVEL_COUNT++;
		this.BBIA_LEVEL_TOP = -1 + BBIA_LEVEL_COUNT;
		this.at = new int[BBIA_LEVEL_COUNT];
		this.sign = BBIA_UNSIGNED;
	}

	// @Constructor for custom bitsize and sign
	public bbia (int bits, bool sign) {
		this.BBIA_LEVEL_COUNT = bits / BBIA_INTEGER_SIZE;
		if (bits % BBIA_INTEGER_SIZE != 0) this.BBIA_LEVEL_COUNT++;
		this.BBIA_LEVEL_TOP = -1 + BBIA_LEVEL_COUNT;
		this.at = new int[BBIA_LEVEL_COUNT];
		this.sign = sign;
	}

	public void sign_change () {
		this.sign = (this.sign) ? false : true;
	}

	public void sign_set (bool sign) {
		this.sign = sign;
	}

	public bool sign_is () {
		return this.sign;
	}

	public int level_value_get (int lvl) {
		if (lvl < 0 || lvl > BBIA_LEVEL_TOP)
			return -1;
		return this.at[lvl];
	}

	void bitshift_left (int value) {

		int[] savedBits = new int[BBIA_LEVEL_TOP-1];

		int[2] bitMask = {0,0};
		for (int curBit = BBIA_INTEGER_SIZE-value+1; curBit <= BBIA_INTEGER_SIZE; curBit++)
			bitMask[0] |= stuaa.bitflag(curBit);

		// 1. Shift with saved bits
		// we shift all levels from top to zero
		// but save the part which is lost
		// zero level not count in saving
		// we saved bits in position INTEGER_SIZE...INTEGER_SIZE-value
		// but need set bits in position value...1

		// 2. Set saved bits
		// for all levels lesser then top
		// we set saved bits

		for (int lvl = BBIA_LEVEL_TOP; lvl >= 0; this.at[lvl] <<= value, lvl--)
		if (lvl > 0) {
			bitMask[1] = this.at[lvl] & bitMask[0];
			stuaa.shiftr (bitMask+1, BBIA_INTEGER_SIZE-value);
			savedBits[lvl-1] = bitMask[1];
		}

		for (int lvl = 0; lvl < BBIA_LEVEL_TOP; lvl++)
			this.at[lvl] |= savedBits[lvl];
	}


}
