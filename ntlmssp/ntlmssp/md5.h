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

namespace crypto {

    util::size const Md5HashSize = util::SizeInBytes(16); //128-bit hash size
    int const md5mod = 56;

    //the md5 hash of no data (e.g. the string "") is a known constant. 
    std::vector<byte> const Md5HashEmptyData = {
        0xd4, 0x1d, 0x8c, 0xd9, 
        0x8f, 0x00, 0xb2, 0x04,
        0xe9, 0x80, 0x09, 0x98,
        0xec, 0xf8, 0x42, 0x7e
    };

    class Md5 {
    public:
        Md5();
        ~Md5() = default;

        std::unique_ptr<std::vector<byte>> calculate_hash(std::vector<byte> const& buffer);
    private:
        util::size const Md5BlockSize = util::SizeInWords32(16); //512-bit block size
        std::vector<uint32_t> state;

        // Implementing 4294967296 * abs(sin(i)) as a table
        std::vector<uint32_t> K = {
            0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
            0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
            0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
            0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
            0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
            0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
            0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
            0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
            0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
            0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
            0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
            0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
            0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
            0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
            0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
            0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
        };

        //Function definition
        using auxfunc = std::function<uint32_t(uint32_t, uint32_t, uint32_t)>;

        //N.B. 'a' is changed by this function
        uint32_t inline circular_lshift(uint32_t const x, int const shift);
        void inline md5round(uint32_t& a, uint32_t const b, uint32_t const c, uint32_t const d, auxfunc func, uint32_t const currword, int const shift, int const TableValue);
        void md5_transform(std::vector<uint32_t> const& block);

        void init();
        uint32_t inline packbytes(byte const msb, byte const mhb, byte const mlb, byte const lsb);
        uint32_t inline packbytes(std::vector<byte> const& bytes);
        void expandbytes(std::vector<uint32_t> const& wordbuffer, std::unique_ptr<std::vector<byte>>& bytebuff);
        void expandbytes(uint32_t const& word, std::vector<byte>& buffer);
        void pagebuffer(std::vector<byte>::const_iterator& iterator, std::vector<uint32_t>& block);
    };
}
