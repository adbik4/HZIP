#pragma once
#include<queue>
#include<unordered_map>
#include <string>

#include "types.h"
#include "file.h"


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
	void printPostOrderHelper(struct Node* node);
	void traverseEncodings(struct Node* node, std::unordered_map<char, Symbol>& map, traversalInfo info);

public:
	// constructor / destructor
	HuffmanTree(const std::unordered_map<char, Symbol>& probMap);
	~HuffmanTree() {
		deletePostOrder(rootNode);
	}

	// methods
	void printPostOrder();
	void encode(std::unordered_map<char, Symbol>& map);
};

// constructor definition
HuffmanTree::HuffmanTree(const std::unordered_map<char, Symbol>& probMap) {
	// THIS IS THE MAIN HUFFMAN CODE LOGIC

	std::priority_queue<Node*, std::vector<Node*>, NodeComp> probQueue;
	for (const auto& [character, value] : probMap) {
		probQueue.push(new Node(value));
	}

	while (probQueue.size() > 1) {
		Node* n1 = probQueue.top();
		probQueue.pop();
		Node* n2 = probQueue.top();
		probQueue.pop();

		Symbol mergedChar(n1->data.freq + n2->data.freq);
		Node* interNode = new Node(mergedChar, n1, n2);

		probQueue.push(interNode);
	}

	rootNode = probQueue.top();
}


// methods definitions
void HuffmanTree::encode(std::unordered_map<char, Symbol>& map) {
	traverseEncodings(rootNode, map, traversalInfo());
}

void HuffmanTree::traverseEncodings(struct Node* node, std::unordered_map<char, Symbol>& map, traversalInfo info) {
	// this function traverses the tree. Every time it takes the left branch it concatenates 0, a right branch it concatenates 1.
	// When it reaches a leaf it saves the encoding at the corresponding entry in the table

	if (node == nullptr) {
		return;
	}
	else if (node->left == nullptr &&
		node->right == nullptr)
	{
		// save
		map[node->data.character].encoding.code = info.encoding;
		map[node->data.character].encoding.length = info.length;
	}

	info.encoding <<= 1;
	info.length += 1;
	traverseEncodings(node->left, map, info);
	info.length -= 1;
	info.encoding >>= 1;

	info.encoding = (info.encoding << 1) + 1;
	info.length += 1;
	traverseEncodings(node->right, map, info);
	info.length -= 1;
	info.encoding >>= 1;
}

void HuffmanTree::printPostOrder() {
	printPostOrderHelper(rootNode);
}

void HuffmanTree::printPostOrderHelper(struct Node* node) {
	if (node == nullptr) {
		return;
	}
	printPostOrderHelper(node->left);
	printPostOrderHelper(node->right);
	if (node->left == nullptr && node->right == nullptr) {
		std::cout << node->data << '\n';
	}
}

void HuffmanTree::deletePostOrder(struct Node* node) {
	if (node == nullptr) {
		return;
	}
	deletePostOrder(node->left);
	deletePostOrder(node->right);
	delete node;
}