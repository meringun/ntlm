#include "netstring.h"
#include <exception>
#include <stdexcept>

namespace util {
    std::unique_ptr<std::vector<byte>> netstring::getbuff() const {
        auto arr = std::make_unique<std::vector<byte>>();
        for (uint16_t const c : str) {
            arr->push_back(getlsb(c));
            arr->push_back(getmsb(c));
        }
        return arr;
    }
}