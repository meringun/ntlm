#include "md5.h"

void Md5::init() {
	//Md5 is initialized to this specific buffer
	state = {
		0x67452301,
		0xefcdab89,
		0x98badcfe,
		0x10325476
	};
}
Md5::Md5() : hash(Md5HashSize,0){
	init();
}

void Md5::md5_transform(vector<uint32_t> block) {
	//round and round we go
	uint32_t a = state[0], b = state[1], c = state[2], d = state[3];
	int TableInput = 1;
	vector<int> BlockPositions;
	vector<BitShifers> ShifterArr;

	// There's a lot of repitition in these calculations: Each round is grouped in 4 batches of 4 for how
	// a,b,c,d and the shifter values cycle, with 4 rounds in total. The only thing that's different between
	// the batches is the way we access the words in our block.

	auto md5cycle = [&](auxfunc func) {
		for (int i = 0; i < 16; i++) {
			md5round(a, b, c, d, func, block[BlockPositions[i]], ShifterArr[0], TableInput++); i++;
			md5round(d, a, b, c, func, block[BlockPositions[i]], ShifterArr[1], TableInput++); i++;
			md5round(c, d, a, b, func, block[BlockPositions[i]], ShifterArr[2], TableInput++); i++;
			md5round(b, c, d, a, func, block[BlockPositions[i]], ShifterArr[3], TableInput++);
		}
	};
	//Round1
	auxfunc F = [](uint32_t const x, uint32_t const y, uint32_t const z) {return (x & y) | (~x & z); };
	BlockPositions = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
	ShifterArr = { Round1a, Round1b, Round1c, Round1d };
	md5cycle(F);

	//Round 2
	auxfunc G = [](uint32_t const x, uint32_t const y, uint32_t const z) { return (x & z) | (y & ~z); };
	BlockPositions = { 1, 6, 11, 0, 5, 10, 15, 4, 9, 14, 3, 8, 13, 2, 7, 12 };
	ShifterArr = { Round2a, Round2b, Round2c, Round2d };
	md5cycle(G);

	//Round 3
	auxfunc H = [](uint32_t const x, uint32_t const y, uint32_t const z) { return (x ^ y & z); };
	BlockPositions = { 5, 6, 11, 14, 1, 4, 7, 10, 13, 0, 3, 6, 9, 12, 15, 2 };
	ShifterArr = { Round3a, Round3b, Round3c, Round3d };
	md5cycle(H);

	//Round 4
	auxfunc I = [](uint32_t const x, uint32_t const y, uint32_t const z) { return (y ^ (x | ~z)); };
	BlockPositions = { 0, 7, 14, 5, 12, 3, 10, 1, 8, 15, 6, 13, 4, 11, 2, 9 };
	ShifterArr = { Round4a, Round4b, Round4c, Round4d };
	md5cycle(I);

	state[0] += a;
	state[1] += b;
	state[2] += c;
	state[3] += d;
}

void Md5::process_message(vector<byte> & const buffer) {
	
}

