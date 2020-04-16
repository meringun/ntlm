#include "md5.h"

namespace crypto {

    void Md5::init() {
        //Md5 is initialized to this specific buffer
        state = {
            0x67452301,
            0xefcdab89,
            0x98badcfe,
            0x10325476
        };
    }
    Md5::Md5() {
        init();
    }
    // e.g.1100110 << 3 == 0110110
    // Handling the case where shift is 0 without using an if()
    uint32_t inline Md5::circular_lshift(uint32_t const x, int const shift) {
        return (x << shift) | (x >> (-shift & 31));
    }

    void inline Md5::md5round(uint32_t& a, uint32_t const b, uint32_t const c, uint32_t const d, auxfunc func, uint32_t const currword, int const shift, int const TableValue) {
        a = b + circular_lshift(a + func(b, c, d) + currword + K[TableValue], shift);
    }

    void Md5::md5_transform(std::vector<uint32_t> const& block) {
        //round and round we go
        uint32_t a = state[0], b = state[1], c = state[2], d = state[3];
        int TableInput = 0;

        // There's a lot of repetition in these calculations: Each round is grouped in 4 batches of 4 for how
        // a,b,c,d and the shifter values cycle, with 4 rounds in total. The only thing that's different between
        // the batches is the way we access the words in our block.
        // Note that TableInput goes from 1 .. 64 sequentially

        struct Md5Cycle {
            std::vector<uint32_t> BlockPosition;
            std::vector<int> BitShift;
            auxfunc CycleFunction;
        };
        std::vector<Md5Cycle> cycles;

        //Round1
        Md5Cycle c1;
        c1.CycleFunction = [](uint32_t const x, uint32_t const y, uint32_t const z) { return (x & y) | (~x & z); };
        c1.BlockPosition = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
        c1.BitShift = { 7, 12, 17, 22 };
        cycles.push_back(c1);

        //Round 2
        Md5Cycle c2;
        c2.CycleFunction = [](uint32_t const x, uint32_t const y, uint32_t const z) { return (x & z) | (y & ~z); };
        c2.BlockPosition = { 1, 6, 11, 0, 5, 10, 15, 4, 9, 14, 3, 8, 13, 2, 7, 12 };
        c2.BitShift = { 5, 9, 14, 20 };
        cycles.push_back(c2);

        //Round 3
        Md5Cycle c3;
        c3.CycleFunction = [](uint32_t const x, uint32_t const y, uint32_t const z) { return (x ^ y ^ z); };
        c3.BlockPosition = { 5, 8, 11, 14, 1, 4, 7, 10, 13, 0, 3, 6, 9, 12, 15, 2 };
        c3.BitShift = { 4, 11, 16, 23 };
        cycles.push_back(c3);

        //Round 4
        Md5Cycle c4;
        c4.CycleFunction = [](uint32_t const x, uint32_t const y, uint32_t const z) { return (y ^ (x | ~z)); };
        c4.BlockPosition = { 0, 7, 14, 5, 12, 3, 10, 1, 8, 15, 6, 13, 4, 11, 2, 9 };
        c4.BitShift = { 6, 10, 15, 21 };
        cycles.push_back(c4);

        for (Md5Cycle const& cl : cycles) {
            for (int i = 0; i < Md5BlockSize.getwords32();) {
                md5round(a, b, c, d, cl.CycleFunction, block[cl.BlockPosition[i]], cl.BitShift[0], TableInput++); i++;
                md5round(d, a, b, c, cl.CycleFunction, block[cl.BlockPosition[i]], cl.BitShift[1], TableInput++); i++;
                md5round(c, d, a, b, cl.CycleFunction, block[cl.BlockPosition[i]], cl.BitShift[2], TableInput++); i++;
                md5round(b, c, d, a, cl.CycleFunction, block[cl.BlockPosition[i]], cl.BitShift[3], TableInput++); i++;
            }
        }

        state[0] += a;
        state[1] += b;
        state[2] += c;
        state[3] += d;
    }

    uint32_t inline Md5::packbytes(byte const lsb, byte const mlb, byte const mhb, byte const msb) {
        return (msb << 24 | mhb << 16 | mlb << 8 | lsb);
    }
    uint32_t inline Md5::packbytes(std::vector<byte> const& bytes) {
        return (packbytes(bytes[0], bytes[1], bytes[2], bytes[3]));
    }
    void Md5::expandbytes(uint32_t const& word, std::vector<byte>& buffer) {
        buffer.push_back(util::getlsb(word));
        buffer.push_back(util::getlob(word));
        buffer.push_back(util::gethob(word));
        buffer.push_back(util::getmsb(word));
    }

    void Md5::expandbytes(std::vector<uint32_t> const& wordbuffer, std::unique_ptr<std::vector<byte>>& bytebuff) {
        for (uint32_t const& word : wordbuffer) {
            expandbytes(word, *bytebuff);
        }
    }

    void Md5::pagebuffer(std::vector<byte>::const_iterator& iterator, std::vector<uint32_t>& block) {
        for (int j = 0; j < Md5BlockSize.getwords32(); j++) {
            auto citr = iterator + j * 4;
            block[j] = packbytes(*citr, *(citr + 1), *(citr + 2), *(citr + 3));
        }
    }


    std::unique_ptr<std::vector<byte>> Md5::calculate_hash(std::vector<byte> const& buffer) {
        if (buffer.empty()) {
            return std::make_unique<std::vector<byte>>(Md5HashEmptyData);
        }

        std::vector<uint32_t> block(Md5BlockSize.getwords32(), 0);

        // We only have to worry about padding on the last block of the buffer, before that we 
        // can just copy it and process.
        auto blockstart = buffer.begin();
        for (; buffer.end() - blockstart > Md5BlockSize.getbytes(); blockstart += Md5BlockSize.getbytes()) {
            pagebuffer(blockstart, block);
            md5_transform(block);
        }

        // We're now at the last block. To make our lives easier we'll use a temporary byte block 
        std::vector<byte> tempbuffer(blockstart, buffer.end());

        int last_pos = (buffer.size() % Md5BlockSize.getbytes());
        int padlength = last_pos < md5mod ? md5mod - last_pos : Md5BlockSize.getbytes() - (last_pos - md5mod);

        tempbuffer.push_back(0x80); padlength--;
        for (int i = 0; i < padlength; i++)
            tempbuffer.push_back(0);

        // Now append the length of the buffer, with length meaning number of bits. 
        // Since we're using a vector, max bit size is 2^32*8 or 2^35. Digest supports arbitrary
        // length but this is good enough for us. 
        uint64_t BufferSize = (uint64_t) buffer.size() * 8;
        expandbytes((BufferSize & 0xFFFFFFFF), tempbuffer);
        expandbytes((BufferSize >> 32) & 0xFFFFFFFF, tempbuffer);

        for (auto citr = tempbuffer.begin(); citr < tempbuffer.end(); citr += Md5BlockSize.getbytes()) {
            pagebuffer(citr, block);
            md5_transform(block);
        }
        std::unique_ptr<std::vector<byte>> hash = std::make_unique<std::vector<byte>>();
        expandbytes(state, hash);

        init(); //refresh the starting state.
        std::fill(tempbuffer.begin(), tempbuffer.end(), 0);
        std::fill(block.begin(), block.end(), 0);

        return hash;
    }
}

