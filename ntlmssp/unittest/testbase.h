#pragma once
#include "string"
#include "vector"

namespace unittest {
	struct test {
	protected:
		std::string testname = "test";
	public:
		test() = default;
		virtual bool run();
	};
}
