#pragma once

#include "common.h"
#include <functional>
#include <math.h>

// N.B. it's almost never a good idea to create your own crypto-library
// instead of using a tried-and-tested source like openssl or Microsoft's
// bcrypt/ncrypt system. You may introduce timing attacks, set IV's incorreclty,
// forget to include counters, or just have hard-to-catch bugs. 
// It's being done here for pure learning purposes.

using namespace std;

// #tosearch: we're assuming here that bytes are 8-bit in our architecture. Technically this
// isn't guaranteed in all architectures, but is this still a concern?
int const Md5HashSize = 16; //128-bit hash size

class Md5 {
private:
	int const Md5BufferSize = 4; //Internally, MD5 uses 4 32-bit words for calculations
	int const Md5BlockSize = 16; //512-bit block size, madeup of 16 32-bit words

	vector<uint32_t> state;
	vector<byte> hash;

	enum BitShifers {
		Round1a = 7,
		Round1b = 12,
		Round1c = 17,
		Round1d = 22,
		Round2a = 5,
		Round2b = 9,
		Round2c = 14,
		Round2d = 20,
		Round3a = 4,
		Round3b = 11,
		Round3c = 16,
		Round3d = 23,
		Round4a = 6,
		Round4b = 10,
		Round4c = 15,
		Round4d = 21
	};
	
	//this should be in a table since this never changes, putting the function for lazyness
	int SinTable(int i) { return int(4294967296 * abs(sin(i))); }

	//Function definition
	using auxfunc = std::function<uint32_t(uint32_t, uint32_t, uint32_t)>;

	//Yes, we do want a to change.
	void md5round(uint32_t & a, uint32_t const b, uint32_t const c, uint32_t const d, auxfunc func, uint32_t const currword, BitShifers shift, int const TableValue) {
		a = b + ((a + func(b, c, d) + currword + SinTable(TableValue)) << shift);
	}
	
	void md5_transform(vector<uint32_t> block);
	void process_message(vector<byte> & const buffer);

	void init();
	void padbuffer();

public:
	Md5();
	~Md5() = default;

	unique_ptr<vector<byte>> calculate_hash(vector<byte> & const buffer);
};
