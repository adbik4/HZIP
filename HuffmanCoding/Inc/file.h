#pragma once
#include <iostream>
#include <string>
#include <unordered_map>
#include <fstream>
#include <array>
#include <memory>

#include "types.h"
#include "tree.h"

class File {  // singleton
private:
	static std::shared_ptr<File> instance;

	static std::array<char, 4> _format;
	static std::vector<char> _content;
	static std::unordered_map<char, Symbol> _huffMap;
	static std::shared_ptr<HuffmanTree> _huffTree;

	// private constructor
	File(std::string filepath);

	// private methods
	std::unordered_map<char, Symbol> CalcFrequency();

public:
	// public constructors
	File(const File&) = delete;
	File& operator=(const File&) = delete;

	static std::shared_ptr<File> getInstance(std::string str) {
		if (!instance) {
			instance = std::shared_ptr<File>(new File(str));
		}
		return instance;
	}

	// methods
	static bitVector compress();
	static std::string decompress(const std::vector<char>& vector);

	static std::tuple<std::array<char, 4>, std::vector<char>, std::shared_ptr<HuffmanTree>>  readHuffFile(const std::string& filepath);
	static void writeFile(const std::string& filepath);


	// getters
	std::string getContent() const { return std::string(_content.begin(), _content.end()); }
	std::unordered_map<char, Symbol> getMapping() const { return _huffMap; }
	std::shared_ptr<HuffmanTree> getTree() const { return _huffTree; }
};

// overloads
std::ostream& operator<<(std::ostream& os, const std::unordered_map<char, Symbol>& map);