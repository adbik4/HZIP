#pragma once
#include <queue>
#include <unordered_map>
#include <string>

#include "types.h"


class HuffmanTree {
private:
	Node* rootNode;
	struct NodeComp {
		bool operator()(Node* self, Node* other) {
			return self->data.freq > other->data.freq;
		}
	};
	
	struct traversalInfo {
		uint32_t encoding = 0;
		uint8_t length = 0;
	};

	// methods
	void deletePostOrder(struct Node* node);
	void traverseEncoding(struct Node* node, std::unordered_map<char, Symbol>& map, traversalInfo info);
	char traverseDecoding(Node* node, bitVector& path, uint32_t& i) const;
	void traverseFlattening(Node* node, bitVector& mask, std::vector<char>& data) const;

public:
	// constructor / destructor
	HuffmanTree(const std::unordered_map<char, Symbol>& probMap);	// constructs tree from scratch
	HuffmanTree(const std::vector<char>& vec, const bitVector& mask);						// reconstructs tree based on flattened data

	~HuffmanTree() {
		deletePostOrder(rootNode);
	}

	// methods
	void encodeTable(std::unordered_map<char, Symbol>& map);
	char decodeChar(bitVector& path, uint32_t& start_idx) const;
	std::pair<bitVector, std::vector<char>> flatten() const;
};