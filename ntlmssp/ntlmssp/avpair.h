#pragma once
#include "util.h"

namespace ntlm {
    
    enum class avid: uint16_t {
        MsvAvEOL = 0x0000,
        MsvAvNbComputerName = 0x0001,
        MsvAvNbDomainName = 0x0002,
        MsvAvDnsComputername = 0x0003,
        MsvAvDnsDomainName = 0x0004,
        MsvAvDnsTreeName = 0x0005,
        MsvAvFlags = 0x0006,
        MsvAvTimestamp = 0x0007,
        MsvAvSingleHost = 0x0008,
        MsvAvTargetName = 0x0009,
        MsvChannelBindings = 0x000a
    };

    struct avpair {
    public:
        enum class flags : uint32_t {
            AccountAuthIsConstrained = 0x0000001,
            MessageIntegrityProvided = 0x0000002,
            SpnIsFromUntrustedSource = 0x0000004
        };
        avpair() = default;
        avpair(avid id, std::vector<byte> buffer);
        avpair(avid id, util::netstring str);
        
        bool is_string() const;
        std::unique_ptr<util::netstring> get_string() const;

        std::vector<byte> get_buffer() const;

    private:
        avid id;
        uint16_t len;
        std::vector<byte> buff;

    };

    struct AvPairsBuilder {
    public:
        void push_string(util::netstring str);
        void push_flag(avpair::flags);
        void push_timestamp();
        void push_singlehost();
        void push_channelbinds();
    private:
        std::vector<avpair> pairs;
    };
}
