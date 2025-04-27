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
	{}

	// private methods
	std::unordered_map<char, Symbol> CalcFrequency();
	void encode();

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
			map.insert({ x, Symbol(x, 1) }); // pierwsze wystąpienie znaku
		}
		else {
			++map[x].freq;	// z każdym kolejnym wystąpieniem
		}
	}
	return map;
}

void File::encode() {
	for (const auto& [key, value] : _huffMap) {
		_huffMap[key].encoding = _huffTree.getEncoding(value.character);
	}
}