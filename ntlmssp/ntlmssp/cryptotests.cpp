#include "common.h"

namespace unittest {
	
	struct Md5Tests {
		std::vector<byte> VectorInput;
		std::string StringInput;
		std::vector<byte> HashResult;
		
		Md5Tests() = default;

		bool run();
	};
}