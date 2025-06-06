#include "file.h"
#include "general.h"

// description:
// This file contains all of the basic functions for reading and writing .huf files

uint32_t readDWORD(std::ifstream& file) {
	std::array<char, 4> buffer;
	uint32_t value;
	file.read(buffer.data(), 4);
	std::memcpy(&value, buffer.data(), 4);  // signature
	return value;
}

std::vector<char> readCharSequence(std::ifstream& file) {
	uint32_t length;
	std::vector<char> length_data(4);
	file.read(length_data.data(), 4);  // mask size in bytes
	std::memcpy(&length, length_data.data(), 4);

	std::vector<char> result(length);
	file.read(result.data(), length); // read actual data
	return result;
}

void writeTree(std::ofstream& file, std::shared_ptr<HuffmanTree> _huffTree) {
	std::vector<char> mask;
	std::vector<char> tree_data;
	std::tie(mask, tree_data) = _huffTree->flatten();
	
	// store information about the nodes
	uint32_t mask_length = toLittleEndian(mask.size()); // length of the data block
	file.write(reinterpret_cast<const char*>(&mask_length), 4);
	file.write(reinterpret_cast<const char*>(mask.data()), mask.size()); // write mask

	// store the actual data
	uint32_t tree_length = toLittleEndian(tree_data.size()); // length of the data block
	file.write(reinterpret_cast<const char*>(&tree_length), 4);
	file.write(tree_data.data(), tree_data.size()); // write tree_data
}

std::tuple<std::array<char, 4>, std::vector<char>, std::shared_ptr<HuffmanTree>>
File::readHuffFile(const std::string& filepath) {
	std::vector<char> content;

	std::ifstream file(filepath, std::ios::binary);
	if (!file.is_open()) {
		std::cout << "ERROR: wrong file path\n";
		throw std::out_of_range("ERROR: wrong file path\n");
	}

	// validate
	std::array<char, 4> format;
	uint32_t signature;
	signature = readDWORD(file); //read signature
	file.read(format.data(), 4); // read format

	if (signature != 0x46465548 || *format.begin() != '.') {
		std::cout << "ERROR: invalid/corrupted input file format\n";
		throw std::out_of_range("ERROR: invalid/corrupted input file format\n");
	}

	// unpack tree and recreate the tree structure
	std::vector<char> mask = readCharSequence(file);
	std::reverse(mask.begin(), mask.end()); // swap endianness
	std::vector<char> tree_data = readCharSequence(file);
	std::shared_ptr<HuffmanTree> tree = std::make_shared<HuffmanTree>(tree_data, mask);

	// read until the end of file, storing everything in a vector
	// decompress

	content = { '<', 'e', 'm', 'p', 't', 'y', '>' };
	file.close();
	return { format, content, tree };
}

void File::writeFile(const std::string& filepath) {
	std::ofstream file(filepath, std::ios::trunc | std::ios::binary);

	// signature ----------
	uint32_t signature = 0x46465548;
	file.write(reinterpret_cast<const char*>(&signature), 4);

	// format -------------
	file.write(reinterpret_cast<const char*>(&_format), 4);

	// pack tree ----------
	writeTree(file, _huffTree);

	// file contents -----
	std::vector<char> data = compress()._data;
	file.write(reinterpret_cast<const char*>(data.data()), data.size());
	file.close();
}