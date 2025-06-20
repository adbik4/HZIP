#include "file.h"
// description: main file method definitions

std::shared_ptr<File> File::instance = nullptr;

std::string File::_format;
std::vector<char> File::_content;
std::unordered_map<char, Symbol> File::_huffMap;
std::shared_ptr<HuffmanTree> File::_huffTree = nullptr;

File::File(const std::filesystem::path& filepath)
{
	if (filepath.extension().string() == ".huf") {
		// --- DECOMPRESSION ---
		try {
			std::tie(_format, _content, _huffTree) = readHuffFile(filepath);
		} catch (const std::exception& e) {
			std::cout << e.what() << '\n';
			std::exit(-1);
		}

		// write back the reconstruted data
		try {
			writeTargetFile(filepath.stem().string().append(_format));
		}
		catch (const std::exception& e) {
			std::cout << e.what() << '\n';
			std::exit(-1);
		}

		std::cout << "File extracted to " << filepath.stem().string().append(_format) << '\n';
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

		_format = filepath.extension().string();

		// perform stochastic analysis of the message
		_huffMap = CalcFrequency();

		// generate the huffman tree
		_huffTree = std::make_shared<HuffmanTree>();

		// encode characters
		_huffTree->encodeTable(_huffMap);

		// write a .huf file
		try {
			writeHuffFile(filepath.stem().string().append(".huf"));
		} catch (const std::exception& e) {
			std::cout << e.what() << '\n';
			std::exit(-1);
		}

		// PRINT SUMMARY
		std::cout << "File compressed to " << filepath.stem().string().append(".huf") << '\n';
		std::cout << this->getMapping() << '\n';
	}
}

std::unordered_map<char, Symbol> File::CalcFrequency() {
	const float N = static_cast<float>(_content.size());
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