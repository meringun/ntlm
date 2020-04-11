#include "utiltests.h"

namespace unittest {
    ConversionTests::ConversionTests(std::vector<byte> const& bytes, std::vector<char> const& chars) {
        if (bytes.size() != chars.size()) {
            throw std::invalid_argument("arrays are not the same size");
        }
        byte_representation = bytes;
        char_representation = chars;
    }
    bool ConversionTests::run() {
        bool passed = std::equal(
            byte_representation.begin(),
            byte_representation.end(),
            char_representation.begin(),
            [](byte const& l, char const& r) {
                return (l == util::hextobyte(r));
            }
        );
        if (!passed) {
            this->errstring = "Error with converting char array into buffer array\nchar array: ";
            for (char const& c : this->char_representation)
                errstring += c;
            errstring += "\n";
        }
        return passed;
    }

    bool test_conversions() {
        testsuit tests("Hex to Byte Conversion");

        std::vector<byte> ints = { 0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9 };
        std::vector<byte> ints2 = { 0xa, 0xb, 0xc, 0xd, 0xe, 0xf };

        std::vector<char> chars1 = { '0','1','2','3','4','5','6','7','8','9' };
        std::vector<char> chars2 = { 'a','b','c','d','e','f' };
        std::vector<char> chars3 = { 'A','B','C','D','E','F' };


        tests.addtest(std::make_unique<ConversionTests>(ints, chars1));
        tests.addtest(std::make_unique<ConversionTests>(ints2, chars2));
        tests.addtest(std::make_unique<ConversionTests>(ints2, chars3));

        return tests.run();
    }
}