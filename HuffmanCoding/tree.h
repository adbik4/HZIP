#pragma once
#include<queue>
#include<unordered_map>
#include <string>

#include "types.h"
#include "file.h"


class HuffmanTree {
private:
	Node* rootNode;

	// methods
	void deletePostOrder(struct Node* node);
	void printPostOrderHelper(struct Node* node);
	void accessPostOrder(struct Node* node);

public:
	// constructor / destructor
	HuffmanTree(const std::unordered_map<char, Symbol>& probMap);
	~HuffmanTree() {
		deletePostOrder(rootNode);
	}

	// methods
	void printPostOrder();
	std::string getEncoding(char c);
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

void HuffmanTree::accessPostOrder(struct Node* node) {
	if (node == nullptr) {
		return;
	}
	accessPostOrder(node->left);
	accessPostOrder(node->right);
}

std::string HuffmanTree::getEncoding( char c) {
	std::string encoding;

	accessPostOrder(rootNode->left);
	accessPostOrder(rootNode->right);

	if (rootNode->left == nullptr &&
		rootNode->right == nullptr &&
		rootNode->data.character == c)
	{
		return encoding;
	}
}