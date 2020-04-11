#pragma once
#include "testbase.h"
#include "md5.h"

namespace unittest {
	struct Md5Test : test {
	private:
		std::vector<byte> testinput, testoutput;
		std::string instring, outstring;
	public:
		Md5Test() : test("md5") {}
		Md5Test(std::string const& input, std::string const& byteoutput);
		bool run();
	};

	bool test_md5();
}
