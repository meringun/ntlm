#include "cryptotests.h"
#include <exception>

namespace unittest {
	
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

		for (auto citr = output.begin(); citr < output.end(); citr += 2) {
			char high = *citr, low = *(citr + 1);
			byte h = util::hextobyte(high), l = util::hextobyte(low);
			testoutput.push_back(h << 4 | l);
		}
	}
	
	bool Md5Test::run() {
		crypto::Md5 hash;
		auto result = hash.calculate_hash(testinput);
		if (testoutput != *result) {
			std::string result_str = "";
			errstring = testname + ": for input " + instring + 
				" we expected " + outstring +
				"; instead we got " + std::string(result->begin(), result->end());
			return false;
		}
		return true;
	}

	bool test_md5() {
		testsuit tests("Md5 Hash Tests");

		//crypto tests
		tests.addtest(make_unique<Md5Test>("", "d41d8cd98f00b204e9800998ecf8427e"));
		tests.addtest(make_unique<Md5Test>("a", "0cc175b9c0f1b6a831c399e269772661"));
		tests.addtest(make_unique<Md5Test>("message digest", "f96b697d7cb7938d525a2f31aaf161d0"));
		tests.addtest(make_unique<Md5Test>(
			"abcdefghijklmnopqrstuvwxyz",
			"c3fcd3d76192e4007dfb496cca67e13b"));
		tests.addtest(make_unique<Md5Test>(
			"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789",
			"d174ab98d277d9f5a5611c2c9f419d9f"));
		tests.addtest(make_unique<Md5Test>(
			"12345678901234567890123456789012345678901234567890123456789012345678901234567890",
			"57edf4a22be3c955ac49da2e2107b67a"));
		
		std::string sonnet1 = "From fairest creatures we desire increase,\n";
		sonnet1 += "That thereby beauty's rose might never die,\n";
		sonnet1 += "But as the riper should by time decease,\n";
		sonnet1 += "His tender heir might bear his memory:\n";
		sonnet1 += "But thou contracted to thine own bright eyes,\n";
		sonnet1 += "Feed'st thy light's flame with self-substantial fuel,\n";
		sonnet1 += "Making a famine where abundance lies,\n";
		sonnet1 += "Thy self thy foe, to thy sweet self too cruel:\n";
		sonnet1 += "Thou that art now the world's fresh ornament,\n";
		sonnet1 += "And only herald to the gaudy spring,\n";
		sonnet1 += "Within thine own bud buriest thy content,\n";
		sonnet1 += "And, tender churl, mak'st waste in niggarding:\n";
		sonnet1 += "Pity the world, or else this glutton be,\n";
		sonnet1 += "To eat the world's due, by the grave and thee.";

		tests.addtest(make_unique<Md5Test>(
			sonnet1,
			"f01456da63f7fae6c678a988500e5ea8"
			));
		return tests.run();
	}
}