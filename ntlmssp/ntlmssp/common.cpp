
#include "common.h"
#include "constants.h"
#

namespace util {
	netstring::netstring(std::wstring& str, nametype type) {
		if ((type == nametype::netbios && str.size() > constants::max_netbios) ||
			(type == nametype::dns && str.size() > constants::max_dns) ||
			(type == nametype::spn && str.size() > constants::max_spn)) {
			throw std::invalid_argument("string is too large");
		}
		this->type = type;
		if (!str.empty()) {
			this->str = str;
		}
	}
}