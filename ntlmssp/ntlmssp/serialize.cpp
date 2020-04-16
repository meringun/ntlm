#include "serialize.h"

namespace ntlm {
    std::ostream& operator<< (std::ostream & os, serialize const& obj) {
        obj.pack(os);
        return os;
    }
    std::istream& operator>> (std::istream & is, serialize& obj) {
        obj.unpack(is);
        return is;
    }
}