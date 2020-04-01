#pragma once

#include "structures.h"

using namespace std;

class message {
protected:
	// #todo change to NtlmSignature[], currently visual studio doesn't like it.
	char const * NtlmSignature = "NTLMSSP";
	unique_ptr<vector<AvPairs>> avpairs = nullptr;
	uint32_t NegotiateFlags = 0;
	
	bool validate_avpairs();
	bool validate();
public:
	message() = default;
	~message() = default;
	unique_ptr<vector<byte>> serialize();
	void add_flag();
};

class NtlmNegotiate : message {
private:
public:

};

class NtlmChallenge : message {
private:
public:

};

class NtlmResponse : message {
private:
public:

};
