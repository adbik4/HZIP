#pragma once
#include <iostream>
#include <string>
#include <unordered_map>
#include <fstream>
#include <string_view>
#include <array>

#include "types.h"
#include "tree.h"

class File {  // singleton
private:
	static File* instance;

	static std::array<char, 4> _format;
	static std::string _content;
	static std::unordered_map<char, Symbol> _huffMap;
	static HuffmanTree* _huffTree;

	// private constructor
	File(std::string filepath)
	{
		std::tie(_format, _content) = std::pair<std::array<char, 4>, std::string>({ {'.', 't', 'x', 't'}, "AAAAABBBCCD"}); 		// TEMPORARY FOR DEBUGGING
		_huffMap = CalcFrequency();
		_huffTree = new HuffmanTree(_huffMap);
		_huffTree->encodeTable(_huffMap);
	}

	//File(std::string filepath)
	//{
	//	std::tie(_format, _content, _huffTree) = readHuffFile(filepath);
	//	_huffMap = CalcFrequency();
	//}

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

	static std::tuple<std::array<char, 4>, std::string, HuffmanTree*>  readHuffFile(const std::string& filepath);
	static void writeFile(const std::string& filepath);


	// getters
	std::string getContent() const { return _content; }
	std::unordered_map<char, Symbol> getMapping() const { return _huffMap; }
	HuffmanTree* getTree() const { return _huffTree; }
};

// overloads
std::ostream& operator<<(std::ostream& os, const std::unordered_map<char, Symbol>& map);