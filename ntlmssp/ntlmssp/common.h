#pragma once

#include <memory>
#include <vector>
#include <cstdint>
#include <string>

typedef unsigned char byte;

namespace helper {

    enum SizeType {
        Byte,
        Word32,
        Word64
    };

    // A helper class to help convert a size into different counts
    // e.g. being able to say how big a buffer is in bytes or words. etc.
    // We could allow bits but then we have to figure out how to handle remainders.
    class size {
    private:
        //internally we're storing the size as number of bytes.
        size_t absolute = 0;
    public:
        size() = default;
        size(size_t const size, SizeType const  type) {
            if (type == Byte)
                absolute = size;
            else if (type == Word32)
                absolute = size * sizeof(uint32_t);
            else if (type == Word64)
                absolute = size * sizeof(uint64_t);
        }
        ~size() = default;

        //we're relying on compilers optimizing this
        size_t getsize(SizeType const type) const {
            if (type == Byte) return getbytes();
            if (type == Word32) return getwords32();
            if (type == Word64) return getwords64();
        }
        size_t getbytes() const { return absolute; }
        size_t getwords32() const { return absolute / sizeof(uint32_t); }
        size_t getwords64() const { return absolute / sizeof(uint64_t); }
    };

    size SizeInBytes(size_t s) { 
        return size(s, SizeType::Byte); 
    }
    size SizeInWords32(size_t s) {
        return size(s, SizeType::Word32);
    }
    size SizeInWords64(size_t s) {
        return size(s, SizeType::Word64);
    }
    
    // when dealing with network names, we don't know whether they're 
    // a netbios mame or fqdn or spn.

    enum NameType {
        Netbios,
        Dns,
        Spn
    };

    class netstring {
    private:
        std::wstring str = L"";
        NameType type = Netbios;
    public:
        netstring() = default;
        ~netstring() = default;

        std::vector<byte> getbuffer() const {
            
        }
    };
}
