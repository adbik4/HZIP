#include "message.h"

template <typename T>
struct Node {
	T data;
	Node* right;
	Node* left;

	Node(const T data) : data(data) { left = right = nullptr; }
};

template <typename T>
class BinaryTree {
private:
	Node<T>* rootNode;

public:
	BinaryTree() : rootNode(nullptr) {};
	~BinaryTree() {
		deletePostOrder(rootNode);
	}

	void deletePostOrder(struct Node<T>* node) {
		if (node == nullptr) {
			return;
		}
		deletePostOrder(node->left);
		deletePostOrder(node->right);
		delete node;
	}

	void accessPostOrder(struct Node<T>* node) {
		if (node == nullptr) {
			return;
		}
		accessPostOrder(node->left);
		accessPostOrder(node->right);
		std::cout << node->data << '\n';
	}

	void printPostOrder() {
		accessPostOrder(rootNode);
	}

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