#include "file.h"
#include "general.h"

// description:
// This file contains all of the basic functions for reading and writing .huf files

// --- Helper functions ----------

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
}

// --- Main IO logic -----------

std::vector<char> File::readSourceFile(const std::filesystem::path& filepath) {
	std::ifstream file(filepath.filename(), std::ios::binary);
	if (!file.is_open()) {
		throw std::out_of_range("ERROR: wrong file path");
	}
	return readUntilEOF(file);
}

void File::writeTargetFile(const std::filesystem::path& filepath) {
	std::ofstream file(filepath.filename(), std::ios::trunc | std::ios::binary);
	file.write(_content.data(), _content.size());

	if (!file) {
		throw std::exception("ERROR: target write failed");
	}

	file.close();
}

std::tuple<std::string, std::vector<char>, std::shared_ptr<HuffmanTree>>
File::readHuffFile(const std::filesystem::path& filepath) {
	std::ifstream file(filepath.filename(), std::ios::binary);
	if (!file.is_open()) {
		throw std::out_of_range("ERROR: wrong file path");
	}

	// validate
	uint32_t signature;
	signature = readDWORD(file); //read signature

	// read format
	std::array<char, 4> buffer;
	file.read(buffer.data(), 4);
	std::string format = ".";
	for (char c : buffer) {
		if (c != '\0') {
			format.insert(format.end(), c);
		}
	}

	if (signature != 0x46465548 || *format.begin() != '.') {
		throw std::out_of_range("ERROR: invalid/corrupted input file format");
	}

	// unpack tree and recreate the tree structure
	std::vector<char> mask = readDataBlock(file);
	std::vector<char> tree_data = readDataBlock(file);
	std::shared_ptr<HuffmanTree> tree = std::make_shared<HuffmanTree>(tree_data, mask);
	
	// leftover bit count
	char leftover;
	file.read(&leftover, 1);

	// read until the end of file
	bitVector bit_data = (bitVector) readUntilEOF(file);
	bit_data._bitIndex = leftover;

	file.close();
	return { format, File::decompress(bit_data, tree), tree };
}

void File::writeHuffFile(const std::filesystem::path& filepath) {
	std::ofstream file(filepath.filename(), std::ios::trunc | std::ios::binary);

	if (!file.good()) {
		throw std::out_of_range("ERROR: unable to create the .huf file");
	}

	// signature ----------
	uint32_t signature = 0x46465548;
	file.write(reinterpret_cast<const char*>(&signature), 4);

	// format -------------
	_format.erase(_format.begin()); // remove the dot
	while (_format.length() < 4) {
		_format.insert(_format.end(), '\0'); // padding
	}
	file.write(reinterpret_cast<const char*>(_format.data()), 4);

	// pack tree ----------
	writeTree(file, _huffTree);

	// file contents -----
	const bitVector compressed_data = compress();
	char leftover = static_cast<const char>(compressed_data._bitIndex);
	file.write(reinterpret_cast<const char*>(&leftover), 1);

	file.write(reinterpret_cast<const char*>(compressed_data._data.data()), compressed_data._data.size()); // actual data
	if (!file.good()) {
		throw std::exception("ERROR: Compress write failed ");
	}

	file.close();
}