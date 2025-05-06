#pragma once
#include <iostream>
#include <string>
#include <vector>

class Code {
public:
	uint32_t code;
	uint8_t length;

	std::string toString() const {
		std::string str;
		uint32_t val = code;

		// convert to binary
		while (val > 0) {
			if (val % 2) {
				str.insert(str.begin(), '1');
			} else {
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
};

class Symbol {
public:
	char character;
	float freq;
	Code encoding;

	Symbol()
		: character('\0'),
		freq(0),
		encoding{}
	{}

	Symbol(const float& frequency)
		: character('\0'),
		 freq(frequency),
		encoding{}
	{}

	Symbol(const char& character, const float& frequency)
		: character(character),
		 freq(frequency),
		 encoding{}
	{}


	// overloads
	friend std::ostream& operator<<(std::ostream& os, const Symbol& sym) {
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

		token.push_back(' ');
		while (token.length() < MAX_TOKEN_LEN) {
			token.push_back('-');
		}

		os << token << "> freq: " << sym.freq << ", encoding: " << sym.encoding.toString();
		return os;
	}
};

struct Node {
	Symbol data;
	Node* right;
	Node* left;

	// constructors
	Node(const Symbol& data)
		: data(data),
		 left(nullptr),
		 right(nullptr)
	{}

	Node(const Symbol& data, Node* left, Node* right)
		: data(data),
		  left(left),
		  right(right)
	{}
};

class bitVector {
private:
	uint8_t bitIndex = 0; // current position in the current byte (0..7)

	// private methods
	void pushBit(bool bit);

public:
	std::vector<uint8_t> data;

	// methods
	void pushBits(uint32_t bits, uint8_t count);
	bool pop_front();
	bool empty();
};