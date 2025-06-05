#include <Windows.h>
#include <iomanip>

#include "file.h"
#include "tree.h"
#include "types.h"

int main()
{
	// init
	SetConsoleOutputCP(1250);
	std::cout << std::fixed << std::setprecision(3);

	// code 
	//File* message = File::getInstance("test.huf");
	//bitVector compressed_data = File::compress();
	//std::cout << message->getMapping() << "\n";
	//std::cout << "decompression result : " << File::decompress(compressed_data) << "\n";

	//auto tree = message->getTree();

	bitVector mask;
	mask.pushBits(0b11011011010, 11);
	std::vector<char> tree_data = { '\0', 'A', '\0', 'B', '\0', 'D', 'C' };
	HuffmanTree* tree = new HuffmanTree(tree_data, mask);

	bitVector mask2;
	std::vector<char> tree_data2;
	std::tie(mask2, tree_data2) = tree->flatten();
	std::cout << "mask: " << mask2 << '\n';
	for (char x : tree_data2) {
		if (x == '\0') {
			std::cout << "[nullptr]\n";
		}
		else {
			std::cout << x << '\n';
		}
	}


	//	 WHAT THE CODE SHOULD LOOK LIKE
	//	 decompress:
	//	File file = openFile("filepath.txt"); // automatically decompresses, reads tables and trees, fills in its properties
	//	std::cout << file.getContent();

	//	 compress:
	//	std::string content = "ABBCCC";
	//	File file(content); // fills in its properties, generates tables and trees
	//	file.writeFile("filepath.txt"); // automatically compresses

	return 0;
}