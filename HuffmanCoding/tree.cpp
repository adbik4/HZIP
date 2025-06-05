#include "tree.h"
// description:

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

HuffmanTree::HuffmanTree(const std::vector<char>& vec) {
	// reconstruct the tree from a flattened list
	//std::vector tmp = vec;
	//while (!tmp.empty()) {

	//}
}

// methods definitions
void HuffmanTree::encodeTable(std::unordered_map<char, Symbol>& map) {
	traverseEncoding(rootNode, map, traversalInfo());
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

char HuffmanTree::decodeChar(bitVector& path) const {
	return traverseDecoding(rootNode, path);
}

char HuffmanTree::traverseDecoding(Node* node, bitVector& path) const {
	// this function traverses the tree using a path as instructions to decode a symbol.
	// Every time it reads zero it takes a left branch, one - a right branch.
	// When it reaches a leaf it returns the decoded character

	if (node->left == nullptr && node->right == nullptr) {
		return node->data.character;
	}

	char decodedChar = '\0';
	if (path.pop_front()) {
		decodedChar = traverseDecoding(node->right, path);
	}
	else {
		decodedChar = traverseDecoding(node->left, path);
	}

	return decodedChar;
}

void HuffmanTree::deletePostOrder(struct Node* node) {
	if (node == nullptr) {
		return;
	}
	deletePostOrder(node->left);
	deletePostOrder(node->right);
	delete node;
}

std::pair<bitVector, std::vector<char>> HuffmanTree::flatten() const {
	//using preorder traversal
	std::vector<char> data;
	bitVector mask;

	traverseFlattening(rootNode, mask, data);

	return { mask, data };
}

void HuffmanTree::traverseFlattening(Node* node, bitVector& mask, std::vector<char>& data) const {
	if (node == nullptr) {
		mask.pushBits(0, 1);
		return;
	}
	data.push_back(node->data.character);
	mask.pushBits(1, 1);
	traverseFlattening(node->left, mask, data);
	traverseFlattening(node->right, mask, data);
}