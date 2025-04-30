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
	static std::string _content;
	static std::unordered_map<char, Symbol> _huffMap;
	static HuffmanTree* _huffTree;

	// private constructor
	File(std::string str)
	{
		_content = str;
		_huffMap = CalcFrequency();
		_huffTree = new HuffmanTree(_huffMap);
		_huffTree->encodeTable(_huffMap);
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

	// methods
	static bitVector compress();
	static std::string decompress(const bitVector& vector);


	// getters
	std::string getContent() const { return _content; }
	std::unordered_map<char, Symbol> getMapping() const { return _huffMap; }
	HuffmanTree* getTree() const { return _huffTree; }
};


// main method definitions
std::unordered_map<char, Symbol> File::CalcFrequency() {
	const double N = static_cast<double>(_content.size());
	std::unordered_map<char, Symbol> map;

	for (char x : _content) {
		if (!map.contains(x)) {
			Symbol newSymbol(x, 1/N);
			map.insert({ x, newSymbol });
		}
		else {
			map[x].freq += 1/N;
		}
	}
	return map;
}

bitVector File::compress() {
	bitVector path;

	for (char c : _content) {
		Code encoding = _huffMap.find(c)->second.encoding;
		path.pushBits(encoding.code, encoding.length);
	}
	return path;
}

std::string File::decompress(const bitVector& vector) {
	bitVector tmpPath = vector;
	std::string content;

	while (!tmpPath.empty()) {
		content.push_back(_huffTree->decodeChar(tmpPath));
	}
	return content;
}


// overloads
std::ostream& operator<<(std::ostream& os, const std::unordered_map<char, Symbol>& map) {
	std::cout << "symbol count: " << map.size() << "\n\n";
	for (const auto& [key, value] : map) {
		std::cout << value << "\n";
	}
	return os;
}