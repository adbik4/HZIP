#include "file.h"
#include "general.h"

// description:
// This file contains all of the basic functions for reading and writing .huf files

uint32_t readDWORD(std::ifstream& file) {
	// read a 32bit value
	std::array<char, 4> buffer;
	uint32_t value;
	file.read(buffer.data(), 4);
	std::memcpy(&value, buffer.data(), 4);  // signature
	return value;
}

std::vector<char> readDataBlock(std::ifstream& file) {
	// reads a single data block
	uint32_t length = readDWORD(file); // data block length
	std::vector<char> result(length); // allocate buffer
	file.read(result.data(), length); // read data
	return result;
}

std::vector<char> readUntilEOF(std::ifstream& file) {
	std::vector<char> raw_data;
	std::array<char, CHUNK_SIZE> buffer;

	while (file) {
		file.read(buffer.data(), buffer.size());
		std::streamsize bytesRead = file.gcount();
		raw_data.insert(raw_data.end(), buffer.begin(), buffer.begin() + bytesRead);
	}
	return raw_data;
}

void writeTree(std::ofstream& file, std::shared_ptr<HuffmanTree> _huffTree) {
	bitVector mask;
	std::vector<char> tree_data;
	std::tie(mask, tree_data) = _huffTree->flatten();
	
	// store information about the nodes
	uint32_t mask_length = toLittleEndian(mask._data.size()); // length of the data block
	file.write(reinterpret_cast<const char*>(&mask_length), 4);
	file.write(reinterpret_cast<const char*>(mask._data.data()), mask._data.size()); // write mask

	// store the actual data
	uint32_t tree_length = toLittleEndian(tree_data.size()); // length of the data block
	file.write(reinterpret_cast<const char*>(&tree_length), 4);
	file.write(tree_data.data(), tree_data.size()); // write tree_data

	file.close();
}

std::vector<char> File::readSourceFile(const std::string& filepath) {
	std::ifstream file(filepath, std::ios::binary);
	if (!file.is_open()) {
		throw std::out_of_range("ERROR: wrong file path");
	}
	return readUntilEOF(file);
}

void File::writeTargetFile(const std::string& filepath) {
	std::ofstream file(filepath, std::ios::trunc | std::ios::binary);
	file.write(_content.data(), _content.size());
	file.close();
}

std::tuple<std::array<char, 4>, std::vector<char>, std::shared_ptr<HuffmanTree>>
File::readHuffFile(const std::string& filepath) {
	std::ifstream file(filepath, std::ios::binary);
	if (!file.is_open()) {
		throw std::out_of_range("ERROR: wrong file path");
	}

	// validate
	uint32_t signature;
	std::array<char, 4> format;
	signature = readDWORD(file); //read signature
	file.read(format.data(), 4); // read format

	if (signature != 0x46465548 || *format.begin() != '.') {
		throw std::out_of_range("ERROR: invalid/corrupted input file format");
	}

	// unpack tree and recreate the tree structure
	std::vector<char> mask = readDataBlock(file);
	std::vector<char> tree_data = readDataBlock(file);
	std::shared_ptr<HuffmanTree> tree = std::make_shared<HuffmanTree>(tree_data, mask);
	
	// overhead bit count
	char overhead;
	file.read(&overhead, 1);

	// read until the end of file
	bitVector bit_data = (bitVector) readUntilEOF(file);
	bit_data._bitIndex = overhead;

	file.close();
	return { format, File::decompress(bit_data, tree), tree };
}

void File::writeHuffFile(const std::string& filepath) {
	std::ofstream file(filepath, std::ios::trunc | std::ios::binary);

	// signature ----------
	uint32_t signature = 0x46465548;
	file.write(reinterpret_cast<const char*>(&signature), 4);

	// format -------------
	file.write(reinterpret_cast<const char*>(&_format), 4);

	// pack tree ----------
	writeTree(file, _huffTree);

	// file contents -----
	const bitVector compressed_data = compress();
	char overhead = static_cast<const char>(compressed_data._bitIndex);

	file.write(&overhead, 1); // the amount of overhead bits
	file.write(reinterpret_cast<const char*>(compressed_data._data.data()), compressed_data._data.size()); // actual data
	
	file.close();
}