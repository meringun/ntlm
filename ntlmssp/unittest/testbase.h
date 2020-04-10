#pragma once
#include <string>
#include <vector>
#include <iostream>


namespace unittest {

	struct test {
	protected:
		std::string const testname;
		std::string errstring;
		test(std::string const name) : testname(name) {}
	public:
		test() = default;
		void printerr() const {
			std::cerr << errstring << std::endl;
		}
		std::string geterr() const {
			return errstring;
		}
		virtual bool run() = 0;
	};

	using testsuit = std::vector<std::unique_ptr<test>>;
}
