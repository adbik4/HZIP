#include "file.h"
#include "general.h"
// description: main file method definitions

std::shared_ptr<File> File::instance = nullptr;

std::string File::_format;
std::vector<char> File::_content;
std::unordered_map<char, Symbol> File::_huffMap;
std::shared_ptr<HuffmanTree> File::_huffTree = nullptr;

File::File(std::filesystem::path input_path, std::filesystem::path output_path)
{
	if (input_path.extension().string() == ".huf") {
		// --- DECOMPRESSION ---
		try {
			std::tie(_format, _content, _huffTree) = readHuffFile(input_path);
		} catch (const std::exception& e) {
			std::cout << e.what() << '\n';
			std::exit(-1);
		}

		// write back the reconstruted data
		std::filesystem::path tmp_path;
		try {
			if (output_path.empty()) {
				tmp_path = input_path;
			}
			else {
				tmp_path = output_path;
			}

			tmp_path.replace_extension(_format);
			writeTargetFile(tmp_path);
		}
		catch (const std::exception& e) {
			std::cout << e.what() << '\n';
			std::exit(-1);
		}

		std::cout << "File extracted to " << tmp_path.string() << '\n';
	}
	else {
		// --- COMPRESSION ---
		try {
			_content = readSourceFile(input_path);
		}
		catch (const std::exception& e) {
			std::cout << e.what() << '\n';
			std::exit(-1);
		}

		_format = input_path.extension().string();

		// perform stochastic analysis of the message
		_huffMap = CalcFrequency();

		// generate the huffman tree
		_huffTree = std::make_shared<HuffmanTree>();

		// encode characters
		_huffTree->encodeTable(_huffMap);

		// write a .huf file
		std::filesystem::path tmp_path;
		try {
			if (output_path.empty()) {
				tmp_path = input_path;
			} else {
				tmp_path = output_path;
			}

			tmp_path.replace_extension(".huff");
			writeHuffFile(tmp_path);
		}
		catch (const std::exception& e) {
			std::cout << e.what() << '\n';
			std::exit(-1);
		}

		// PRINT SUMMARY
		std::cout << this->getMapping() << '\n';
		std::cout << "File compressed to " << tmp_path.string() << '\n';
		memStats(input_path, tmp_path);
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