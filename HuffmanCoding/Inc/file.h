#pragma once
#include <iostream>
#include <string>
#include <unordered_map>
#include <fstream>
#include <filesystem>
#include <array>
#include <memory>

#include "types.h"
#include "tree.h"

class File {  // singleton
private:
	static std::shared_ptr<File> instance;

	static std::string _format;
	static std::vector<char> _content;
	static std::unordered_map<char, Symbol> _huffMap;
	static std::shared_ptr<HuffmanTree> _huffTree;

	// private constructor
	File(std::filesystem::path input_path, std::filesystem::path output_path);

	// private methods

public:
	// public constructors
	File(const File&) = delete;
	File& operator=(const File&) = delete;

	static std::shared_ptr<File> getInstance(const std::string& str1, const std::string& str2) {
		if (!instance) {
			instance = std::shared_ptr<File>(new File(str1, str2));
		}
		return instance;
	}

	// methods
	static std::unordered_map<char, Symbol> CalcFrequency();
	static bitVector compress();
	static std::vector<char> decompress(const bitVector& enc_data, std::shared_ptr<HuffmanTree> huffTree);

	static std::vector<char> readSourceFile(const std::filesystem::path& filepath);
	static std::tuple<std::string, std::vector<char>, std::shared_ptr<HuffmanTree>> readHuffFile(const std::filesystem::path& filepath);
	static void writeHuffFile(const std::filesystem::path& filepath);
	static void writeTargetFile(const std::filesystem::path& filepath);


	// getters
	std::vector<char> getContents() const { return _content; }
	std::unordered_map<char, Symbol> getMapping() const { return _huffMap; }
	std::shared_ptr<HuffmanTree> getTree() const { return _huffTree; }
};

// overloads
std::ostream& operator<<(std::ostream& os, const std::unordered_map<char, Symbol>& map);