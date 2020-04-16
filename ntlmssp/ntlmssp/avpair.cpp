#include "avpair.h"
#include <limits>
#include <stdexcept>

namespace ntlm {
    avpair::avpair(avid id, std::vector<byte> buffer) {
        if (buffer.size() > UINT16_MAX)
            throw std::invalid_argument("buffer is too large");
        
    }

    bool avpair::is_string() const {
        return (id == avid::MsvAvNbComputerName ||
                id == avid::MsvAvNbDomainName ||
                id == avid::MsvAvDnsComputername ||
                id == avid::MsvAvDnsDomainName ||
                id == avid::MsvAvDnsTreeName ||
                id == avid::MsvAvTargetName);
    }

    std::unique_ptr<util::netstring> avpair::get_string() const{
        if (!is_string())
            return nullptr;
        util::nameform type;

        if (id == avid::MsvAvNbComputerName ||
            id == avid::MsvAvNbDomainName) {
            type = util::nameform::netbios;
        }
        else if (id == avid::MsvAvDnsComputername ||
                 id == avid::MsvAvDnsDomainName ||
                 id == avid::MsvAvDnsTreeName) {
            type = util::nameform::dns;
        }
        else {
            type = util::nameform::spn;
        }
        return std::make_unique<util::netstring>(type, buff);
    }
}