#pragma once

#include "common.h"
#include <string>

using namespace std;

enum NegotiateFlags {
    End
};

struct FileTime {
    uint32_t LowDateTime = 0;
    uint32_t HighDateTime = 0;
};

struct AvPairs {
private:
    uint16_t avid = 0;
    uint16_t avlen = 0;
    unique_ptr<vector<byte>> avbuff = nullptr;

    void reset() noexcept;
    void copy_buffer(shared_ptr<vector<byte>> buffer);
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
    AvPairs(shared_ptr<vector<byte>> buffer);
    AvPairs(AvId id, shared_ptr<vector<byte>> buffer);
    ~AvPairs() = default;

    void set_id(AvId id) noexcept;
    void set_buffer(shared_ptr<vector<byte>> buffer);
    bool validate();

    unique_ptr<vector<byte>> serialize();
};

//Every AvPair has different data that can be added, broken into different groups:
// 1. unicode strings that are NOT null-terminated, such as machine name. This is the most common data.
// 2. flags, in the case of AvFlags
// 3. an MD5 hash
// 4. FILETIME structure
// 5. Single host Data structure.
// there are also some AvPairs that MUST be included. 
// Let's make our lives easier to have a builder with some more knowledge on the data.

struct AvPairsBuilder {
private:
    //A little optimazation: an MsvAvEOL pair is just a word-sized 0.
    uint32_t MsvEolPair = 0;
    unique_ptr<vector<AvPairs>> CurrentList = nullptr;
public:
    AvPairsBuilder();
    ~AvPairsBuilder();

    void add_computer_name(util::netstring & const name);
    void add_domain_name(util::netstring& const name);
    void add_tree_name(util::netstring& const name);
    void add_target_name(util::netstring& const name);
    void add_filetime(FileTime& const time);
    bool validate();

    unique_ptr<vector<AvPairs>> get_list();
    unique_ptr<vector<byte>> serialize();
};
