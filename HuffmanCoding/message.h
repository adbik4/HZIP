#pragma once

#include <iostream>
#include <string>
#include <queue>
#include <utility>
#include <unordered_map>
#include <iomanip>

class Symbol {
public:
	char character;
	double probability;
	uint32_t encoding;

	Symbol() = default;
	Symbol(char c, double d) : character(c), probability(d), encoding(0) {};

	bool operator<(const Symbol& other) const {
		return probability < other.probability;
	}

	friend std::ostream& operator<<(std::ostream& os, const Symbol& sym) {
		os << sym.character << ": " << sym.probability << '\n';
		return os;
	}
};

class File {
private:
	static File* instance;
	std::string _content;
	std::unordered_map<char, Symbol> _huffMap;

	File(std::string str) {
		_content = str;
		_huffMap = CalcProbability();
	}

	// private methods
	std::unordered_map<char, Symbol> CalcProbability() {
		const double N = _content.size();
		std::unordered_map<char, Symbol> map;

		for (char x : _content) {
			if (!map.contains(x)) {
				map.insert({ x, Symbol(x, 1/N) }); // pierwsze wystąpienie znaku
			}
			else {
				map[x].probability += 1/N;	// z każdym kolejnym wystąpieniem
			}
		}
		return map;
		/*for (const auto& [key, value] : charCount) {
			static_cast<double>(value) / N;
		}*/
	}

public:
	File(const File&) = delete;
	File& operator=(const File&) = delete;

	static File* getInstance(std::string str) {
		if (!instance) {
			instance = new File(str);
		}
		return instance;
	}

	// getters
	std::string GetContent() const { return _content; }
	std::unordered_map<char, Symbol> GetMapping() const { return _huffMap; }
};

// overloads
std::ostream& operator<<(std::ostream& os, std::unordered_map<char, Symbol> other) {
	os << "LISTA PRAWDOPODOBIEŃSTW" << "\n";

	const auto default_precision{ os.precision() };
	os << std::setprecision(2);

	for (const auto& [key, value] : other) {
		if (value.character == ' ') {
			os << "[space]";
			os << ": " << value.probability << '\n';
		}
		else {
			os << value.character;
			os << "      : " << value.probability << '\n';
		}
	}
	os << std::setprecision(default_precision);
	return os;
}