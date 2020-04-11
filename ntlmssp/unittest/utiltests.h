#pragma once
#include "testbase.h"
#include "common.h"

namespace unittest {
    struct ConversionTests : test {
    private:
        std::vector<byte> byte_representation;
        std::vector<char> char_representation;
    public:
        ConversionTests() : test("conversions") {}
        ConversionTests(std::vector<byte> const& bytes, std::vector<char> const& chars);
        bool run();
    };

    bool test_conversions();
}
