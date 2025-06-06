#include "types.h"
#include "general.h"
// description:

// method definitions
std::string Code::toString() const {
	std::string str;
	uint32_t val = code;

	// convert to binary
	while (val > 0) {
		if (val % 2) {
			str.insert(str.begin(), '1');
		}
		else {
			str.insert(str.begin(), '0');
		}
		val >>= 1;
	}

	// pad with zeros
	while (str.size() < length) {
		str.insert(str.begin(), '0');
	}

	return str;
}

bitVector::bitVector(const std::vector<char> vec) {
	_data = vec;
}

void bitVector::pushBit(bool bit) {
	if (_data.size() == 0 && bitIndex == 0) {
		_data.push_back(0);
	}
	if (bit) {
		_data.back() |= (1 << (7 - bitIndex));
	}

	++bitIndex;
	if (bitIndex == 8) {
		bitIndex = 0;
		_data.push_back(0);
	}
}

void bitVector::pushBits(uint32_t bits, uint8_t count) {
	for (int i = count - 1; i >= 0; --i) {
		pushBit((bits >> i) & 1);
	}
}

bool bitVector::empty() {
	return _data.empty();
}

uint32_t bitVector::getLength() const {
	// length of the data in bits
	return 8 * (_data.size()-1) + bitIndex; 
}

bool bitVector::operator[](const uint32_t& idx) const {
	// {{0,1,2,3,4,5,6,7}, {8,9,10,11,12,13,14,15}, ...}
	return _data.at(idx / BYTE_LEN) & (1 << (BYTE_LEN - 1 - idx % BYTE_LEN));
}

std::string bitVector::toString() const {
	std::string result;
	uint32_t curr_len = this->getLength();

	for (uint8_t byte : _data) {
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

		if (curr_len > 8) {
			// itermediate bytes
			// pad with zeros from the left
			while (curr_len < 8) {
				substr.insert(substr.begin(), '0');
			}
			result.append(substr + ' ');
			curr_len -= 8;
		}
		else {
			// last byte
			while (substr.size() >= curr_len) {
				substr.pop_back();
			}
			result.append(substr);
		}
	}
	return result;
}

std::ostream& operator<<(std::ostream& os, const bitVector& bitv) {
	os << bitv.toString();
	return os;
}

std::ostream& operator<<(std::ostream& os, const Symbol& sym) {
	std::string token = std::string(1, sym.character);

	const size_t MAX_TOKEN_LEN = 8;  // to accomodate for the special symbol representations
	if (sym.character == ' ') {
		token = "[space]";
	}
	else if (sym.character == '\n') {
		token = "[newl]";
	}
	else if (sym.character == '\t') {
		token = "[tab]";
	}
	else if (sym.character == '\0') {
		token = "[null]";
	}

	token.push_back(' ');
	while (token.length() < MAX_TOKEN_LEN) {
		token.push_back('-');
	}

	os << token << "> freq: " << sym.freq << ", encoding: " << sym.encoding.toString();
	return os;
}