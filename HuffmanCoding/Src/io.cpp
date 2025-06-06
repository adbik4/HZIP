#include "file.h"
#include "general.h"

// description:
// This file contains all of the basic functions for reading and writing .huf files

void writeTree(std::ofstream& file, std::unique_ptr<HuffmanTree> _huffTree) {
	bitVector mask;
	std::vector<char> tree_data;
	std::tie(mask, tree_data) = _huffTree->flatten();
	
	// store information about the nodes
	uint32_t mask_length = toLittleEndian(mask.data.size()); // length of the data block
	file.write(reinterpret_cast<char*>(&mask_length), 4);
	file.write(reinterpret_cast<char*>(mask.data.data()), mask.data.size()); // write mask

	// store the actual data
	uint32_t tree_length = toLittleEndian(tree_data.size()); // length of the data block
	file.write(reinterpret_cast<char*>(&tree_length), 4);
	file.write(tree_data.data(), tree_data.size()); // write tree_data
}

std::tuple<std::array<char, 4>, std::vector<char>, std::unique_ptr<HuffmanTree>>
File::readHuffFile(const std::string& filepath) {
	std::array<char, 4> format;
	std::vector<char> buffer;
	std::vector<char> content;
	uint32_t value;

	std::ifstream file(filepath, std::ios::binary);
	if (!file.is_open()) {
		std::cout << "ERROR: wrong file path\n";
		throw std::out_of_range("ERROR: wrong file path\n");
	}

	// validate
	buffer.resize(4);
	file.read(buffer.data(), 4);
	std::memcpy(&value, buffer.data(), 4);  // signature

	// read format
	file.read(format.data(), 4);
	if (value != 0x46465548 || *format.begin() != '.') {
		std::cout << "ERROR: invalid/corrupted input file format\n";
		throw std::out_of_range("ERROR: invalid/corrupted input file format\n");
	}

	// unpack tree
	std::vector<char> mask;
	std::vector<char> tree_data;

	file.read(buffer.data(), 4);  // mask size in bytes
	std::memcpy(&value, buffer.data(), 4);
	buffer.resize(value);

	file.read(buffer.data(), value); // read mask
	mask = buffer;

	file.read(buffer.data(), 4);  // tree size in bytes
	std::memcpy(&value, buffer.data(), 4);
	buffer.resize(value);

	file.read(buffer.data(), value); // read tree
	tree_data = buffer;

	// recreate the tree structure
	std::unique_ptr<HuffmanTree> tree = std::make_unique<HuffmanTree>(tree_data, mask);

	// read until the end of file, storing everything in a vector
	// decompress

	//content = 

	return { format, content, std::move(tree) };
}

void File::writeFile(const std::string& filepath) {
	std::ofstream file(filepath, std::ios::trunc | std::ios::binary);

	// signature ----------
	uint32_t signature = 0x46465548;
	file.write(reinterpret_cast<char*>(&signature), 4);

	// format -------------
	file.write(reinterpret_cast<char*>(&_format), 4);

	// pack tree ----------
	writeTree(file, std::move(_huffTree));

	// file contents -----
	std::vector<uint8_t> data = compress().data;
	file.write(reinterpret_cast<char*>(data.data()), data.size());
}