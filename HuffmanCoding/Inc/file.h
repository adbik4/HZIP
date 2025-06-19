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
	static std::unordered_map<char, Symbol> CalcFrequency();
	static bitVector compress();
	static std::vector<char> decompress(const bitVector& enc_data, std::shared_ptr<HuffmanTree> huffTree);

	static std::vector<char> readSourceFile(const std::string& filepath);
	static std::tuple<std::array<char, 4>, std::vector<char>, std::shared_ptr<HuffmanTree>>  readHuffFile(const std::string& filepath);
	static void writeHuffFile(const std::string& filepath);
	static void writeTargetFile(const std::string& filepath);


	// getters
	std::vector<char> getContents() const { return _content; }
	std::unordered_map<char, Symbol> getMapping() const { return _huffMap; }
	std::shared_ptr<HuffmanTree> getTree() const { return _huffTree; }
};

// overloads
std::ostream& operator<<(std::ostream& os, const std::unordered_map<char, Symbol>& map);