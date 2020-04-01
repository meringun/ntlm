#pragma once

#include <memory>
#include <vector>
#include <cstdint>

typedef unsigned char byte;

// When providing a string to a target or resource, it could either
// be in Netbios or Dns/FQDN format. Functions that accept targets
// should have this to verify what type the caller is passing.
enum TargetNameType {
	Netbios,
	Dns,
	End
};
