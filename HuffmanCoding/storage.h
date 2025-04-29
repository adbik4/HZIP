#pragma once
#include <vector>
#include <string>
#include <unordered_map>

#include "types.h"


class bitVector {
private:
	std::vector<uint8_t> data;
    uint8_t bitIndex = 0; // current position in the current byte (0..7)

    // private methods
    void pushBit(bool bit);

public:
    // methods
    void pushBits(uint32_t bits, uint8_t count);
    void writeToFile(const std::string& filename) const;
};


// method definitions
void bitVector::pushBit(bool bit) {
    if (bitIndex == 0) {
        data.push_back(0);
    }
    if (bit) {
        data.back() |= (1 << (7 - bitIndex));
    }

    ++bitIndex;
    bitIndex &= 7;
}

void bitVector::pushBits(uint32_t bits, uint8_t count) {
    for (int i = count - 1; i >= 0; --i) {
        pushBit((bits >> i) & 1);
    }
}

void bitVector::writeToFile(const std::string& filename) const {
    // left to implement
}

// standalone functions
bitVector compress(const std::string& content, const std::unordered_map<char, Symbol>& huffMap) {
    bitVector vector;
    for (char c : content) {
        Code encoding = huffMap.find(c)->second.encoding;
        vector.pushBits(encoding.code, encoding.length);
    }
    return vector;
}