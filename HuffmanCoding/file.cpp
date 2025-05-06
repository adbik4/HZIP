#include "file.h"
// description: main method definitions

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

std::string File::decompress(const bitVector& vector) {
	bitVector tmpPath = vector;
	std::string content;

	while (!tmpPath.empty()) {
		content.push_back(_huffTree->decodeChar(tmpPath));
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