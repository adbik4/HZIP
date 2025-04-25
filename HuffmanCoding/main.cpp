#include <Windows.h>
#include "message.h"
#include "tree.h"

int main()
{
	SetConsoleOutputCP(1250);
	
	BinaryTree<int> tree;
	tree.insert(1);
	tree.insert(4);
	return 0;
}
