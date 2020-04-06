#pragma once

#include <memory>
#include <vector>
#include <cstdint>
#include <string>

typedef unsigned char byte;

namespace util {

    enum class sizeunit {
        byte,
        word32,
        word64
    };

    // A helper class to help convert a size into different counts
    // e.g. being able to say how big a buffer is in bytes or words. etc.
    // We could allow bits but then we have to figure out how to handle remainders.
    class size {
    private:
        // internally we're storing the size as number of bytes.
        size_t absolute = 0;
    public:
        size() = default;
        size(size_t const size, sizeunit const unit) {
            if (unit == sizeunit::byte)
                absolute = size;
            else if (unit == sizeunit::word32)
                absolute = size * sizeof(uint32_t);
            else if (unit == sizeunit::word64)
                absolute = size * sizeof(uint64_t);
        }
        ~size() = default;

        //we're relying on compilers optimizing this
        size_t getsize(sizeunit const unit) const {
            if (unit == sizeunit::byte) return getbytes();
            if (unit == sizeunit::word32) return getwords32();
            if (unit == sizeunit::word64) return getwords64();
        }
        size_t getbytes() const { return absolute; }
        size_t getwords32() const { return absolute / sizeof(uint32_t); }
        size_t getwords64() const { return absolute / sizeof(uint64_t); }
    };

    size SizeInBytes(size_t s) { 
        return size(s, sizeunit::byte); 
    }
    size SizeInWords32(size_t s) {
        return size(s, sizeunit::word32);
    }
    size SizeInWords64(size_t s) {
        return size(s, sizeunit::word64);
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
