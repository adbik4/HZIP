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
	// cut out the last 4 characters (the file extension)
	std::array<char, 4> extension;
	std::string_view view = std::string_view(filepath).substr(filepath.size() - 4);
	std::copy_n(view.begin(), 4, extension.begin());

	if (extension == std::array<char, 4>({'.', 'h', 'u', 'f'})) {
		// decompression
		std::tie(_format, _content, _huffTree) = readHuffFile(filepath);
		// find a way to recreate a new file with the original files data
	}
	else {
		// compression
		//std::tie(_format, _content) = readSourceFile();
		_format = std::array<char, 4>({ '.', 't', 'x', 't' });
		std::string text = "AAAAABBBCCD";
		_content = std::vector<char>(text.begin(), text.end());
		_huffMap = CalcFrequency();
		_huffTree = std::make_shared<HuffmanTree>(_huffMap);
		_huffTree->encodeTable(_huffMap);
		writeFile("test_file.huf");
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

std::string File::decompress(const std::vector<char>& vector) {
	std::string content;

	uint32_t start_idx = 0;
	while (start_idx < vector.size()) {
		content.push_back(_huffTree->decodeChar(vector, start_idx));
	}
	return content;
}

// overloads
std::ostream& operator<<(std::ostream& os, const std::unordered_map<char, Symbol>& map) {
	std::cout << "symbol count: " << map.size() << "\n\n";
	for (const auto& [key, value] : map) {
		std::cout << value << "\n";
	}
	return os;
}