#pragma once
#include <iostream>
#include <string>
#include <vector>

#define BYTE_LEN 8

class Code {
public:
	uint32_t code;
	uint8_t length;

	std::string toString() const;
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
	bool empty();
	uint32_t getLength() const;
	std::string toString() const;

	//overloads
	bool operator[](int idx);
};

//overloads
std::ostream& operator<<(std::ostream& os, const bitVector& bitv);
std::ostream& operator<<(std::ostream& os, const Symbol& sym);