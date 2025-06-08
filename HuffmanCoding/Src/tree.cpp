#include "tree.h"
#include "file.h"
#include "general.h"
#include <stack>
#include <cassert>

// description:

// constructor definition
HuffmanTree::HuffmanTree() {
	// contructor for creating a tree for compression
	// THIS IS THE MAIN HUFFMAN CODE LOGIC

	// calculate the frequencies of each character
	std::unordered_map<char, Symbol> probMap = File::CalcFrequency();

	// generate a queue of Symbol nodes sorted by probability 
	std::priority_queue<Node*, std::vector<Node*>, NodeComp> probQueue;
	for (const auto& [character, value] : probMap) {
		probQueue.push(new Node(value));
	}

	// while there are still nodes in the queue, merge them into branches based on their probability.
	// the most common Symbols end up closer to the root node which in turn gives them shorter encodings
	while (probQueue.size() > 1) {
		Node* n1 = probQueue.top();
		probQueue.pop();
		Node* n2 = probQueue.top();
		probQueue.pop();

		Symbol mergedChar(n1->data.freq + n2->data.freq);
		Node* interNode = new Node(mergedChar, n1, n2);

		probQueue.push(interNode);
	}

	// now the root node is a handle to the entire tree
	rootNode = probQueue.top();
}

HuffmanTree::HuffmanTree(const std::vector<char>& tree_data, const bitVector& mask) {
	// contructor for recreating a tree for decompression
	// reconstruct the tree from a flattened list
	// ---------
	// tree_data -> list of node contents in preorder traversal order
	// mask -> sequence of bits that encodes branch endings:
	// 1 = node is a character,
	// 0 = node is null

	uint32_t data_idx = 1;
	uint32_t mask_idx = 1;
	std::stack<Node*> s;

	rootNode = new Node(Symbol(tree_data[0]));
	s.push(rootNode); // root node

	while (!s.empty()) {
		Node* currNode = s.top();
		if (mask[mask_idx]) {
			// mask 1:
			// continue
			Node* newNode = new Node(Symbol(tree_data[data_idx]));

			if (currNode->left == nullptr) {
				currNode->left = std::move(newNode);
				s.push(newNode);
				++data_idx;
				++mask_idx;
			}
			else if (currNode->right == nullptr) {
				currNode->right = std::move(newNode);
				s.push(newNode);
				++data_idx;
				++mask_idx;
			}
			else {
				s.pop(); // node is filled and can be removed from the stack
			}
		}
		else {
			// mask 0:
			s.pop(); // end branch
			++mask_idx;
		}
	}
}

// methods definitions
void HuffmanTree::encodeTable(std::unordered_map<char, Symbol>& map) {
	if (rootNode->data.character != '\0') {
		// in the case where the tree has only one node, traversal makes no sense.
		map[rootNode->data.character].encoding.code = 1;
		map[rootNode->data.character].encoding.length = 1;
	}
	else {
		traverseEncoding(rootNode, map, traversalInfo());
	}
}

void HuffmanTree::traverseEncoding(struct Node* node, std::unordered_map<char, Symbol>& map, traversalInfo info) {
	// this function traverses the tree.
	// Every time it takes the left branch it concatenates 0, a right branch it concatenates 1.
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
	traverseEncoding(node->left, map, info);
	info.length -= 1;
	info.encoding >>= 1;

	info.encoding = (info.encoding << 1) + 1;
	info.length += 1;
	traverseEncoding(node->right, map, info);
	info.length -= 1;
	info.encoding >>= 1;
}

char HuffmanTree::decodeChar(const bitVector& path, uint32_t& start_idx) const {
	// public method for accessing traverseDecoding() and rootNode
	char decoded_char = traverseDecoding(rootNode, path, start_idx);
	return decoded_char;
}

void HuffmanTree::deletePostOrder(struct Node* node) {
	if (node == nullptr) {
		return;
	}
	deletePostOrder(node->left);
	deletePostOrder(node->right);
	delete node;
}

char HuffmanTree::traverseDecoding(Node* node, const bitVector& path, uint32_t& i) const {
	// this function traverses the tree using a path as instructions to decode a symbol.
	// Every time it reads zero it takes a left branch, one - a right branch.
	// When it reaches a leaf it returns the decoded character

	if (node->left == nullptr && node->right == nullptr) {
		return node->data.character;
	}

	char decodedChar = '\0';
	if (path[i]) {
		++i;
		decodedChar = traverseDecoding(node->right, path, i);
	}
	else {
		++i;
		decodedChar = traverseDecoding(node->left, path, i);
	}
	return decodedChar;
}

std::pair<bitVector, std::vector<char>> HuffmanTree::flatten() const {
	//using preorder traversal
	std::vector<char> data;
	bitVector mask;

	traverseFlattening(rootNode, mask, data);

	return { mask, data};
}

void HuffmanTree::traverseFlattening(Node* node, bitVector& mask, std::vector<char>& data) const {
	// fills in data and mask with information about the tree
	assert(node != nullptr && "Null node encountered in full binary tree");

	mask.pushBit(1);
	data.push_back(node->data.character);

	// Leaf node — encode as "1" + "0"
	if (!node->left && !node->right) {
		mask.pushBit(0);  // Mark as a leaf
		return;
	}

	traverseFlattening(node->left, mask, data);
	traverseFlattening(node->right, mask, data);
}