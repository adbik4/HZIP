#include "message.h"

template <typename T>
class Node {
private:
	T data;
	Node* right;
	Node* left;

public:
	Node(const T data, const Node* r, const Node* l) : data(data) {};
};

template <typename T>
class BinaryTree {
private:
	Node<T>* rootNode;

public:
	BinaryTree() : rootNode(nullptr) {};

	void insert(const T& data) {
		Node<T>* newNode = new Node<T>(data);
		if (rootNode == nullptr) {
			rootNode = newNode;
			return;
		}
		
		std::queue<Node<T>*> queue;
		queue.push(rootNode);
		
		while (!queue.empty()) {
			Node<T>* currNode = queue.front();
			queue.pop();
			if (currNode->left == nullptr) {
				currNode->left = newNode;
				return;
			}
			else {
				queue.push(currNode->left);
			}
			if (currNode->right == nullptr) {
				currNode->right = newNode;
				return;
			}
			else {
				queue.push(currNode->right);
			}
		}
	}
};