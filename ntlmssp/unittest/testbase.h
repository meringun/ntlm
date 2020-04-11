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

    struct testsuit {
    private:
        std::string suit_name = "";
        std::vector<std::unique_ptr<test>> testarr;
    public:
        testsuit() = default;
        testsuit(std::string const& name) : suit_name(name) {};

        void addtest(std::unique_ptr<test> t) {
            testarr.push_back(std::move(t));
        }

        bool run() {
            std::cout << "testsuit \"" << suit_name << "\" is now starting\n";
            int passed = 0;
            for (auto const& t : testarr) {
                if (t->run())
                    passed++;
                else
                    t->printerr();
            }
            std::cout << "testsuit \""<< suit_name << "\" result: " << passed << "/" << testarr.size() << " passed\n";
            return passed == testarr.size();
        }

    };
}
