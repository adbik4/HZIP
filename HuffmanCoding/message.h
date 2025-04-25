#pragma once

#include <iostream>
#include <string>
#include <queue>
#include <utility>
#include <unordered_map>
#include <iomanip>

struct Symbol {
	char character;
	double probability;

	bool operator<(const Symbol& other) const {
		return probability < other.probability;
	}

	friend std::ostream& operator<<(std::ostream& os, const Symbol& sym) {
		os << sym.character << ": " << sym.probability << '\n';
		return os;
	}
};


class Message {
private:
	std::string _content;
	std::priority_queue<Symbol> _probQueue;

	// private methods
	std::priority_queue<Symbol> CalcProbability() const {
		std::priority_queue<Symbol> probQueue;
		const double N = _content.size();
		std::unordered_map<char, int> charCount;

		for (char x : _content) {
			if (!charCount.contains(x)) {
				charCount.insert({ x, 1 }); // pierwsze wystąpienie znaku
			}
			else {
				charCount[x]++;	// z każdym kolejnym wystąpieniem
			}
		}

		for (const auto& [key, value] : charCount) {
			probQueue.push({key, static_cast<double>(value) / N});
		}
		return probQueue;
	}

public:
	// constructors
	Message() {
		_content = "";
		_probQueue = CalcProbability();
	}
	Message(std::string str) {
		_content = str;
		_probQueue = CalcProbability();
	}
	~Message() = default;

	// getters
	std::string GetContent() const { return _content; }
	std::priority_queue<Symbol> GetProbabilities() const { return _probQueue; }

};

// overloads
std::ostream& operator<<(std::ostream& os, std::priority_queue<Symbol> other) {
	os << "LISTA PRAWDOPODOBIEŃSTW" << "\n";

	const auto default_precision{ os.precision() };
	os << std::setprecision(2);

	auto tmp = other;
	for (; !tmp.empty(); tmp.pop()) {
		char chr = tmp.top().character;
		double prob = tmp.top().probability;
		if (chr == ' ') {
			os << "[space]";
			os << ": " << prob << '\n';
		}
		else {
			os << chr;
			os << "      : " << prob << '\n';
		}
	}
	os << std::setprecision(default_precision);
	return os;
}