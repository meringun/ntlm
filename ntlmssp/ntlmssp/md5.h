#pragma once

#include "common.h"
#include <functional>
#include <math.h>
#include <string>

// N.B. it's almost never a good idea to create your own crypto-library
// instead of using a tried-and-tested source like openssl or Microsoft's
// bcrypt/ncrypt system. You may introduce timing attacks, set IV's incorreclty,
// forget to include counters, or just have hard-to-catch bugs. 
// It's being done here for pure learning purposes.

using namespace std;

// #tosearch: we're assuming here that bytes are 8-bit in our architecture. Technically this
// isn't guaranteed in all architectures, but is this still a concern?
helper::size const Md5HashSize = helper::SizeInBytes(16); //128-bit hash size

class Md5 {
private:
    helper::size const Md5BlockSize = helper::SizeInWords32(16); //512-bit block size
    vector<uint32_t> state;
    vector<byte> hash;

    //this should be in a table since this never changes, putting the function for lazyness
    int SinTable(int i) { return int(4294967296 * abs(sin(i))); }

    //Function definition
    using auxfunc = std::function<uint32_t(uint32_t, uint32_t, uint32_t)>;

    //Yes, we do want a to change.
    void md5round(uint32_t & a, uint32_t const b, uint32_t const c, uint32_t const d, auxfunc func, uint32_t const currword, int const shift, int const TableValue) {
        a = b + ((a + func(b, c, d) + currword + SinTable(TableValue)) << shift);
    }
    
    void md5_transform(vector<uint32_t> & const block);

    void init();
    void padbuffer();

public:
    Md5();
    ~Md5() = default;

    unique_ptr<vector<byte>> calculate_hash(vector<byte> & const buffer);
    unique_ptr<vector<byte>> calculate_hash(string& const s);
};
