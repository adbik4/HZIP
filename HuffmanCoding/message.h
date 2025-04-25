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


// utility functions
void printProbability(const std::priority_queue<Symbol>& queue) {
	std::cout << "LISTA PRAWDOPODOBIEŃSTW" << "\n";

	const auto default_precision{ std::cout.precision() };
	std::cout << std::setprecision(2);

	auto tmp = queue;
	for (; !tmp.empty(); tmp.pop()) {
		char chr = tmp.top().character;
		double prob = tmp.top().probability;
		if (chr == ' ') {
			std::cout << "[space]";
			std::cout << ": " << prob << '\n';
		}
		else {
			std::cout << chr;
			std::cout << "      : " << prob << '\n';
		}
	}
	std::cout << std::setprecision(default_precision);
}