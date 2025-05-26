#include "types.h"
// description:

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

bool bitVector::pop_front() {
	if (data.empty()) {
		std::cout << "ERROR: Cannot pop from empty bitVector.\n";
		throw std::out_of_range("Cannot pop from empty bitVector.");
	}

	bool firstElement = *data.begin() & 0x80;
	for (size_t i = 0; i < data.size(); ++i) {
		if (i != data.size() - 1) {
			// shift left by one bit (with carry)
			data.at(i) = (data.at(i) << 1) + (data.at(i + 1) >> 7);
		}
		else {
			// shift left by one bit
			data.at(i) <<= 1;
		}
	}

	--bitIndex;
	bitIndex &= 7;
	if (bitIndex == 0) {
		data.erase(data.end() - 1);
	}
	return firstElement;
}

bool bitVector::empty() {
	return data.empty();
}

std::string bitVector::toString() const {
	std::string result;
	for (uint8_t byte : data) {
		std::string substr;
		// convert to binary
		while (byte > 0) {
			if (byte % 2) {
				substr.insert(substr.begin(), '1');
			}
			else {
				substr.insert(substr.begin(), '0');
			}
			byte >>= 1;
		}

		// pad with zeros
		while (substr.size() < 8) {
			substr.insert(substr.begin(), '0');
		}
		result.append(' ' + substr);
	}
	return result;
}