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
		std::tie(_format, _content) = readFile(filepath);
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

	static std::pair< std::array<char, 4>, std::string >  readFile(const std::string& filepath);
	static void writeFile(const std::string& filepath);


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

std::pair< std::array<char, 4>, std::string > File::readFile(const std::string& filepath) {
	std::array<char, 4> format;
	std::string content;
	
	std::string_view view = std::string_view(filepath).substr(filepath.size() - 4);
	std::copy_n(view.begin(), 4, format.begin());

	if (*format.begin() != '.') {
		std::cout << "ERROR: invalid input file format\n";
		throw std::out_of_range("ERROR: invalid input file format\n");
	}

	// convert char[4] to uint32_t
	//for (size_t i = 0; i < fileformat.size(); ++i) {
	//	uint8_t c = fileformat.at(i);
	//	type += c << ((fileformat.size() - i - 1) * 8);
	//}

	return { format, "content" };
}

void File::writeFile(const std::string& filepath) {
	std::fstream file(filepath, std::ios::trunc | std::ios::out | std::ios::binary);

	// signature
	uint32_t signature = 0x46465548;
	file.write(reinterpret_cast<char*>(&signature), 4);

	// format
	std::array<char, 4> format = { '.', 't', 'x', 't' };
	file.write(reinterpret_cast<char*>(&format), 4);
	
	// tree length
	uint32_t tree_length = ntohl(sizeof(_huffTree)); // little endian
	file.write(reinterpret_cast<char*>(&tree_length), 4);

	file.close();
}


// overloads
std::ostream& operator<<(std::ostream& os, const std::unordered_map<char, Symbol>& map) {
	std::cout << "symbol count: " << map.size() << "\n\n";
	for (const auto& [key, value] : map) {
		std::cout << value << "\n";
	}
	return os;
}