#pragma once
#include <iostream>
#include <string>
#include <queue>
#include <utility>
#include <unordered_map>

#include "types.h"
#include "tree.h"


class File {  // singleton
private:
	static File* instance;
	std::string _content;
	std::unordered_map<char, Symbol> _huffMap;
	HuffmanTree _huffTree;

	// private constructor
	File(std::string str)
		: _content(str),
		_huffMap(CalcFrequency()),
		_huffTree(_huffMap)
	{
		_huffTree.encode(_huffMap);
	}

	// private methods
	std::unordered_map<char, Symbol> CalcFrequency();

public:
	// public constructors
	File(const File&) = delete;
	File& operator=(const File&) = delete;

	static File* getInstance(std::string str) {
		if (!instance) {
			instance = new File(str);
		}
		return instance;
	}


	// getters
	std::string GetContent() const { return _content; }
	std::unordered_map<char, Symbol> GetMapping() const { return _huffMap; }
	HuffmanTree GetTree() const { return _huffTree; }
};


// main method definitions
std::unordered_map<char, Symbol> File::CalcFrequency() {
	const size_t N = _content.size();
	std::unordered_map<char, Symbol> map;

	for (char x : _content) {
		if (!map.contains(x)) {
			Symbol newSymbol(x, 1);
			map.insert({ x, newSymbol }); // pierwsze wystąpienie znaku
		}
		else {
			++map[x].freq;	// z każdym kolejnym wystąpieniem
		}
	}
	return map;
}

// overloads
std::ostream& operator<<(std::ostream& os, const std::unordered_map<char, Symbol>& map) {
	std::cout << "symbol count: " << map.size() << "\n\n";
	for (const auto& [key, value] : map) {
		std::cout << value << "\n";
	}
	return os;
}