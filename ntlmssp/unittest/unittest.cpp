#include <iostream>
#include "cryptotests.h"

using namespace unittest;

bool runtests() {
	std::vector<unique_ptr<test>> testsuit;

	//crypto tests
	testsuit.push_back(make_unique<Md5Test>("", "d41d8cd98f00b204e9800998ecf8427e"));
	testsuit.push_back(make_unique<Md5Test>("a", "0cc175b9c0f1b6a831c399e269772661"));
	testsuit.push_back(make_unique<Md5Test>("abc", "900150983cd24fb0d6963f7d28e17f72"));
	testsuit.push_back(make_unique<Md5Test>("message digest","f96b697d7cb7938d525a2f31aaf161d0"));
	testsuit.push_back(make_unique<Md5Test>(
		"abcdefghijklmnopqrstuvwxyz", 
		"c3fcd3d76192e4007dfb496cca67e13b"));
	testsuit.push_back(make_unique<Md5Test>(
		"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789", 
		"d174ab98d277d9f5a5611c2c9f419d9f"));
	testsuit.push_back(make_unique<Md5Test>(
		"12345678901234567890123456789012345678901234567890123456789012345678901234567890", 
		"57edf4a22be3c955ac49da2e2107b67a"));
	
	int passed = 0;
	for (auto const & t : testsuit) {
		if (t->run())
			passed++;
		else
			t->printerr();
	}

	cout << passed << "/" << testsuit.size() << " tests passed";
	return passed == testsuit.size();
}

int main()
{
}
