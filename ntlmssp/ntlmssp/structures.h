#pragma once

#include "common.h"
#include <string>

// #todo: this header and its definition are incomplete. 

struct FileTime {
    uint32_t LowDateTime = 0;
    uint32_t HighDateTime = 0;
};

struct AvPairs {
private:
    uint16_t avid = 0;
    uint16_t avlen = 0;
    std::vector<byte> avbuff;

    void reset();
public:
    enum AvId {
        AccountAuthenticationIsConstrained = 0x1 << 1,
        MicAvailable = 0x1 << 2,
        UntrustedTargetSpn = 0x1 << 3,
        End
    };

    enum AvFlags {
        MsvAvlEOL = 0,
        MsvAvNbComputerName = 1,
        MsvAvNbDomainName = 2,
        MsvAvDnsComputerName = 3,
        MsvAvDnsDomainName = 4,
        MsvAvDnsTreeName = 5,
        MsvAvFlags = 6,
        MsvAvTimestamp = 7,
        MsvAvSingleHost = 8,
        MsvAvTargetName = 9,
        MsvChannelBindings = 10,
        End
    };

    AvPairs() = default;
    ~AvPairs() = default;

    void set_id(AvId id);
    void set_buffer(std::vector<byte> buffer);
    bool validate();

    std::unique_ptr<std::vector<byte>> serialize();
};

//Every AvPair has different data that can be added, broken into different groups:
// 1. unicode strings that are NOT null-terminated, such as machine name. This is the most common data.
// 2. flags, in the case of AvFlags
// 3. an MD5 hash
// 4. FILETIME structure
// 5. Single host Data structure.
// there are also some AvPairs that MUST be included. 

// #todo: implement this object.
struct AvPairsBuilder {
private:
    //A little optimazation: an MsvAvEOL pair is just a word-sized 0.
    uint32_t MsvEolPair = 0;
    std::vector<AvPairs> CurrentList;
public:
    AvPairsBuilder();
    ~AvPairsBuilder();

    void add_computer_name(util::netstring& const name);
    void add_domain_name(util::netstring& const name);
    void add_tree_name(util::netstring& const name);
    void add_target_name(util::netstring& const name);
    void add_filetime(FileTime& const time);
    bool validate();

    std::unique_ptr<std::vector<AvPairs>> get_list();
    std::unique_ptr<std::vector<byte>> serialize();
};
