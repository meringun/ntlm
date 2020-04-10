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

namespace crypto {

    util::size const Md5HashSize = util::SizeInBytes(16); //128-bit hash size
    int const md5mod = 56;
    
    //the md5 hash of no data (e.g. the string "") is a known constant. 
    vector<byte> const Md5HashEmptyData = { 
        0xd4, 0x1d, 0x8c, 0xd9, 
        0x8f, 0x00, 0xb2, 0x04,
        0xe9, 0x80, 0x09, 0x98,
        0xec, 0xf8, 0x42, 0x7e 
    };

    class Md5 {
    private:
        util::size const Md5BlockSize = util::SizeInWords32(16); //512-bit block size
        vector<uint32_t> state;

        //this should be in a table since this never changes, putting the function for lazyness
        uint32_t SinTable(int i) {
            return 4294967296 * abs(sin(i));
        }

        //Function definition
        using auxfunc = std::function<uint32_t(uint32_t, uint32_t, uint32_t)>;

        //N.B. 'a' is changed by this function
        uint32_t inline circular_lshift(uint32_t const x, int const shift);
        void inline md5round(uint32_t& a, uint32_t const b, uint32_t const c, uint32_t const d, auxfunc func, uint32_t const currword, int const shift, int const TableValue);
        void md5_transform(vector<uint32_t> const & block);

        void init();
        uint32_t inline packbytes(byte const msb, byte const mhb, byte const mlb, byte const lsb);
        uint32_t inline packbytes(vector<byte> const& bytes);
        void expandbytes(vector<uint32_t> const& wordbuffer, unique_ptr<vector<byte>> & bytebuff);
        void expandbytes(uint32_t const& word, vector<byte>& buffer);
        void pagebuffer(vector<byte>::const_iterator & iterator, vector<uint32_t>& block);

    public:
        Md5();
        ~Md5() = default;

        unique_ptr<vector<byte>> calculate_hash(vector<byte> const & buffer);
    };
}
