#include "file.h"
#include <string_view>
// description: main file method definitions

std::shared_ptr<File> File::instance = nullptr;

std::array<char, 4> File::_format;
std::vector<char> File::_content;
std::unordered_map<char, Symbol> File::_huffMap;
std::shared_ptr<HuffmanTree> File::_huffTree = nullptr;

File::File(std::string filepath)
{
	// isolate the last 4 characters (the file extension)
	// ASSUMPTION: the file extension is 4 characters long
	std::array<char, 4> extension;
	std::string_view view = std::string_view(filepath).substr(filepath.size() - 4);
	std::copy_n(view.begin(), 4, extension.begin());

	if (extension == std::array<char, 4>({'.', 'h', 'u', 'f'})) {
		// --- DECOMPRESSION ---
		try {
			std::tie(_format, _content, _huffTree) = readHuffFile(filepath);
		} catch (const std::exception& e) {
			std::cout << e.what() << '\n';
			std::exit(-1);
		}

		// write back the reconstruted data
		std::string new_filepath = filepath;
		new_filepath.resize(filepath.length() - 4);
		new_filepath.append(std::string(_format.begin(), _format.end()));
		try {
			writeTargetFile(new_filepath);
		}
		catch (const std::exception& e) {
			std::cout << e.what() << '\n';
			std::exit(-1);
		}

		std::cout << "File extracted to " << new_filepath << '\n';
	}
	else {
		// --- COMPRESSION ---
		try {
			_content = readSourceFile(filepath);
		}
		catch (const std::exception& e) {
			std::cout << e.what() << '\n';
			std::exit(-1);
		}
		_format = extension;

		// perform stochastic analysis of the message
		_huffMap = CalcFrequency();

		// generate the huffman tree
		_huffTree = std::make_shared<HuffmanTree>();

		// encode characters
		_huffTree->encodeTable(_huffMap);

		// write a .huf file
		std::string new_filepath = filepath;
		new_filepath.resize(filepath.length() - 4);
		new_filepath.append(".huf");
		try {
			writeHuffFile(new_filepath);
		} catch (const std::exception& e) {
			std::cout << e.what() << '\n';
			std::exit(-1);
		}

		std::cout << "File compressed to " << new_filepath << '\n';
	}
}

std::unordered_map<char, Symbol> File::CalcFrequency() {
	const double N = static_cast<double>(_content.size());
	std::unordered_map<char, Symbol> map;

	for (char x : _content) {
		if (!map.contains(x)) {
			Symbol newSymbol(x, 1 / N);
			map.insert({ x, newSymbol });
		}
		else {
			map[x].freq += 1 / N;
		}
	}
	return map;
}

bitVector File::compress() {
	bitVector compressed_data;

	for (char c : _content) {
		Code encoding = _huffMap.find(c)->second.encoding;
		compressed_data.pushBits(encoding.code, encoding.length);
	}
	return compressed_data;
}

std::vector<char> File::decompress(const bitVector& enc_data, std::shared_ptr<HuffmanTree> huffTree) {
	std::vector<char> content;
	uint32_t start_idx = 0;
	while (start_idx < enc_data.getLength()) {
		content.push_back(huffTree->decodeChar(enc_data, start_idx));
	}
	return content;
}