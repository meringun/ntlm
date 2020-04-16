#pragma once
#include <string>
#include "common.h"
#include "constants.h"

namespace util {

    enum class nameform {
        netbios,
        dns,
        fqdn,
        spn
    };

    struct netstring {
    public:
        int max_len;

        netstring() = default;
        netstring(nameform type, std::string const& str);
        netstring(nameform type, std::u16string const& str);
        netstring(nameform type, std::vector<byte> const& buffer);

        std::unique_ptr<std::vector<byte>> getbuff() const;

    protected:
        // wchar_t is 16-bits for Windows and 32-bit for Linux. Let's be specific. 
        // ntlm uses utf-16 in Little-Endian mode.
        std::u16string str;
        nameform type;

        virtual bool validate() const;
    };

    struct netbios_str: netstring {
    public:
        netbios_str();
    private: 
        const int maxlen = constants::netbios_maxlen;
    };

    struct dns_str: netstring {
    public:
        dns_str();
    private:
        const int maxlen = constants::dns_maxlen;
        
    };

    struct spn_str: netstring {
    public:
        spn_str();
    private:
        const int maxlen = constants::spn_maxlen;
    };
}
