#include "cryptotests.h"
#include <exception>

namespace unittest {
	Md5Test::Md5Test(std::vector<byte> const& input, std::vector<byte> const& output) {
		testinput = input;
		testoutput = output;
	}

	// Input is a regular string. Output is the digest of the hash in string format,
	// i.e. if the hash is 0xdef123... the string is "def123..."
	Md5Test::Md5Test(std::string const& input, std::string const& output) {
		if (output.empty() || output.size() != crypto::Md5HashSize.getbytes() * 2) {
			throw std::exception("output is malformed");
		}
		
		//save to make our lives easier if we hit an error
		instring = input;
		outstring = output;

		for (char const c : input)
			testinput.push_back(c);
		// #todo this should be it's own function
		for (auto citr = output.begin(); citr < output.end(); citr += 2) {
			char high = *citr, low = *(citr + 1);
			byte h = util::hextobyte(high), l = util::hextobyte(low);
			testoutput.push_back(h << 8 | l);
		}
	}
	
	bool Md5Test::run() {
		crypto::Md5 hash;
		auto result = hash.calculate_hash(testinput);
		if (testoutput != *result) {
			errstring = testname + ": for input ";
			if (instring.empty() && outstring.empty()) {
				// #todo this is clunky, clean it up
				errstring += std::string(testinput.begin(), testinput.end()) + " we expected "
					+ std::string(testoutput.begin(), testoutput.end()) + "; instead we got " +
					std::string(result->begin(), result->end());
			}
			else {
				// #todo this is clunky, clean it up
				errstring += instring + " we expected " + outstring +
					"; instead we got " + std::string(result->begin(), result->end());
			}
			return false;
		}
		return true;
	}
}