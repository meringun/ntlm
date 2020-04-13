#include "cryptotests.h"
#include <exception>
#include <stdexcept>
#include <sstream>
#include <iomanip>

namespace unittest {

    // Input is a regular string. Output is the digest of the hash in string format,
    // i.e. if the hash is 0xdef123... the string is "def123..."
    Md5Test::Md5Test(std::string const& input, std::string const& output) : test("md5") {
        if (output.empty() || output.size() != crypto::Md5HashSize.getbytes() * 2) {
            throw std::invalid_argument("Parameter \"output\" is malformed");
        }

        //save to make our lives easier if we hit an error
        instring = input;
        outstring = output;

        for (char const c : input)
            testinput.push_back(c);

        for (auto citr = output.begin(); citr < output.end(); citr += 2) {
            char high = *citr, low = *(citr + 1);
            byte h = util::hextobyte(high), l = util::hextobyte(low);
            testoutput.push_back(h << 4 | l);
        }
    }

    bool Md5Test::run() {
        crypto::Md5 hash;
        auto result = hash.calculate_hash(testinput);

        if (testoutput == *result) 
            return true;
        
        std::stringstream ss;
        ss << testname << ": for input \"" << instring
           << "\" we expected \"" << outstring
           << "\"; instead we got \"";
        ss << std::hex;
        for (byte const b : *result) 
            ss << std::setw(2) << std::setfill('0') << (int)b;
        ss << "\"";

        errstring = ss.str();
        return false;
    }

    bool test_md5() {
        testsuit tests("Md5 Hash Tests");

        //crypto tests
        tests.addtest(std::make_unique<Md5Test>("", "d41d8cd98f00b204e9800998ecf8427e"));
        tests.addtest(std::make_unique<Md5Test>("a", "0cc175b9c0f1b6a831c399e269772661"));
        tests.addtest(std::make_unique<Md5Test>("message digest", "f96b697d7cb7938d525a2f31aaf161d0"));
        tests.addtest(std::make_unique<Md5Test>(
            "abcdefghijklmnopqrstuvwxyz",
            "c3fcd3d76192e4007dfb496cca67e13b"));
        tests.addtest(std::make_unique<Md5Test>(
            "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789",
            "d174ab98d277d9f5a5611c2c9f419d9f"));
        tests.addtest(std::make_unique<Md5Test>(
            "12345678901234567890123456789012345678901234567890123456789012345678901234567890",
            "57edf4a22be3c955ac49da2e2107b67a"));

        std::string const sonnet1 = "From fairest creatures we desire increase,\n"
            "That thereby beauty's rose might never die,\n"
            "But as the riper should by time decease,\n"
            "His tender heir might bear his memory:\n"
            "But thou contracted to thine own bright eyes,\n"
            "Feed'st thy light's flame with self-substantial fuel,\n"
            "Making a famine where abundance lies,\n"
            "Thy self thy foe, to thy sweet self too cruel:\n"
            "Thou that art now the world's fresh ornament,\n"
            "And only herald to the gaudy spring,\n"
            "Within thine own bud buriest thy content,\n"
            "And, tender churl, mak'st waste in niggarding:\n"
            "Pity the world, or else this glutton be,\n"
            "To eat the world's due, by the grave and thee.";

        tests.addtest(std::make_unique<Md5Test>(
            sonnet1,
            "f01456da63f7fae6c678a988500e5ea8"
            ));
        return tests.run();
    }
}