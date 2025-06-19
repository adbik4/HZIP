#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <memory>

// DATA TYPE OVERVIEW:
// Code -> used to efficiently store small bits of binary data (up to 32 bits)
// bitVector -> used to store huge amounts of binary data (up to 536 MB)
// Symbol -> holds every information about a single character
// Node -> used to store Symbols inside of a tree

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

	Symbol(const char& character)
		: character(character),
		freq(0.0),
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
public:
	std::vector<char> _data;
	uint8_t _bitIndex = 0; // current position in the current byte (0..7)

	// methods
	bitVector() = default;
	bitVector(const std::vector<char> vec);
	void pushBit(bool bit);
	void pushBits(uint32_t bits, uint8_t count);
	bool empty();
	std::string toString() const;

	// getters / setters
	uint32_t getLength() const;


	//overloads
	bool operator[](const uint32_t& idx) const;
};

//overloads
std::ostream& operator<<(std::ostream& os, const bitVector& bitv);
std::ostream& operator<<(std::ostream& os, const Symbol& sym);