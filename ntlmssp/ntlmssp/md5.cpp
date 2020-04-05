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

    void inline Md5::md5round(uint32_t& a, uint32_t const b, uint32_t const c, uint32_t const d, auxfunc func, uint32_t const currword, int const shift, int const TableValue) {
        a = b + ((a + func(b, c, d) + currword + SinTable(TableValue)) << shift);
    }

    void Md5::md5_transform(vector<uint32_t>& const block) {
        //round and round we go
        uint32_t a = state[0], b = state[1], c = state[2], d = state[3];
        int TableInput = 1;

        // There's a lot of repitition in these calculations: Each round is grouped in 4 batches of 4 for how
        // a,b,c,d and the shifter values cycle, with 4 rounds in total. The only thing that's different between
        // the batches is the way we access the words in our block.
        // Note that TableInput goes from 1 .. 64 sequentially

        struct Md5Cycle {
            vector<uint32_t> BlockPosition;
            vector<int> BitShift;
            auxfunc CycleFunction;
        };
        vector<Md5Cycle> cycles;

        //Round1
        Md5Cycle c1;
        c1.CycleFunction = [](uint32_t const x, uint32_t const y, uint32_t const z) {return (x & y) | (~x & z); };
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
        c3.CycleFunction = [](uint32_t const x, uint32_t const y, uint32_t const z) { return (x ^ y & z); };
        c3.BlockPosition = { 5, 6, 11, 14, 1, 4, 7, 10, 13, 0, 3, 6, 9, 12, 15, 2 };
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

    unique_ptr<vector<byte>> Md5::calculate_hash(vector<byte>& const buffer) {
        if (buffer.empty()) {
            return make_unique<vector<byte>>(Md5HashEmptyData);
        }

        vector<uint32_t> block(Md5BlockSize.getwords32(),0);
        
                    

        // Now append the length of the buffer originally, with length meaning number of bits. 
        // currently this doesn't handle the condition of the message itself being 2^61 bytes or 
        // 2.3 exabytes, but that's fine.
        uint64_t BufferSize = buffer.size() * 8;
        block.push_back(BufferSize & 0xFFFFFFFF);
        block.push_back((BufferSize >> 32) & 0xFFFFFFFF);

    }

    unique_ptr<vector<byte>> Md5::calculate_hash(string& const s) {
        if (s.empty()) {
            return make_unique<vector<byte>>(Md5HashEmptyData);
        }
        
    }

    unique_ptr<vector<byte>> Md5::calculate_hash(unique_ptr<byte> const buffer, uint64_t const bitsize) {
        if (buffer == nullptr || bitsize == 0) {
            return make_unique<vector<byte>>(Md5HashEmptyData);
        }
    }
}

