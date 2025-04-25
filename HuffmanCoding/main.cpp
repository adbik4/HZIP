#include <Windows.h>
#include "message.h"
#include "tree.h"

int main()
{
	SetConsoleOutputCP(1250);
	
	//BinaryTree<int> tree;
	//for (int x : {1,2,3,4,5,6,7}) {
	//	tree.insert(x);
	//}

	//tree.printPostOrder();

	Message msg("Hello World!");
	std::cout << msg.GetProbabilities() << "\n";

	return 0;
}