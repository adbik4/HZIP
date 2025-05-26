#include "file.h"
// description:

std::tuple<std::array<char, 4>, std::string, HuffmanTree*>
File::readHuffFile(const std::string& filepath) {
	std::array<char, 4> format;
	std::array<char, 4> buffer32;
	std::string content;
	uint32_t value;

	std::ifstream file(filepath, std::ios::binary);
	if (!file.is_open()) {
		std::cout << "ERROR: wrong file path\n";
		throw std::out_of_range("ERROR: wrong file path\n");
	}

	// validate
	file.read(buffer32.data(), 4);
	std::memcpy(&value, buffer32.data(), 4);  // signature
	if (value != 0x46465548 || *format.begin() != '.') {
		std::cout << "ERROR: invalid/corrupted input file format\n";
		throw std::out_of_range("ERROR: invalid/corrupted input file format\n");
	}

	// read format
	file.read(format.data(), 4);

	// unpack tree
	file.read(buffer32.data(), 4);  // tree size in bytes
	std::memcpy(&value, buffer32.data(), 4);

	HuffmanTree* tree;

	return { format, "content", tree };
}

void File::writeFile(const std::string& filepath) {
	//// cut out the last 4 characters (the file extension)
	//std::string_view view = std::string_view(filepath).substr(filepath.size() - 4);
	//std::copy_n(view.begin(), 4, format.begin());

	std::ofstream file(filepath, std::ios::trunc | std::ios::binary);

	// signature ----------
	uint32_t signature = 0x46465548;
	file.write(reinterpret_cast<char*>(&signature), 4);

	// format -------------
	std::array<char, 4> format = { '.', 't', 'x', 't' };
	file.write(reinterpret_cast<char*>(&format), 4);

	// pack tree ----------
	bitVector mask;
	std::vector<char> tree_data;
	std::tie(mask, tree_data) = _huffTree->flatten();
	// RETHINK THIS ENTIRE PART !!!!
	uint32_t tree_length = ntohl(tree_data.size()); // little endian

	file.write(reinterpret_cast<char*>(&tree_length), 4);
	file.write(tree_data.data(), tree_data.size());

	// file contents -----
	std::vector<uint8_t> data = compress().data;
	file.write(reinterpret_cast<char*>(data.data()), data.size());

	file.close();
}