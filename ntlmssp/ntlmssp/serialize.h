#pragma once
#include "common.h"
#include <iostream>

namespace ntlm {
    struct serialize {
    public:
        friend std::ostream& operator<< (std::ostream& os, serialize const& obj);
        friend std::istream& operator>> (std::istream& is, serialize& obj);
    protected:
        virtual void pack(std::ostream& os) const = 0;
        virtual void unpack(std::istream& os) = 0;
    };

    struct bytestream: std::istream, std::ostream {

    };
}
