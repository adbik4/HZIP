#include <vector>

class BitVector {
private:
	std::vector<uint8_t> data;
    uint8_t bitIndex = 0; // current position in the current byte (0..7)

public:
    void pushBit(bool bit) {
        if (bitIndex == 0) {
            data.push_back(0);
        }
        if (bit) {
            data.back() |= (1 << (7 - bitIndex));
        }

        ++bitIndex;
        bitIndex &= 7;
    }

	void pushBits(uint32_t bits, uint8_t count) {
        for (int i = count - 1; i >= 0; --i) {
            pushBit((bits >> i) & 1);
        }
	}

	void writeToFile(const std::string& filename) const {
        // left to implement
	}
};