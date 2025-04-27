#pragma once


class Symbol {
public:
	char character;
	size_t freq;
	std::string encoding;

	Symbol()
		: character('\0'),
		freq(0),
		encoding(0)
	{}

	Symbol(const size_t& frequency)
		: character('\0'),
		 freq(frequency),
		 encoding(0)
	{}

	Symbol(const char& character, const size_t& frequency)
		: character(character),
		 freq(frequency),
		 encoding(0)
	{}

	// overrides
	bool operator<(const Symbol& other) const {
		return freq < other.freq;
	}

	friend std::ostream& operator<<(std::ostream& os, const Symbol& sym) {
		const size_t MAX_LEN = 8;
		std::string token = std::string(1, sym.character);
		if (sym.character == ' ') {
			token = "[space]";
		}

		token.push_back(' ');
		while (token.length() < MAX_LEN) {
			token.push_back('-');
		}

		os << token << "-> freq: " << sym.freq << ", encoding: " << sym.encoding;
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

struct NodeComp {
	bool operator()(Node* self, Node* other) {
		return self->data.freq > other->data.freq;
	}
};