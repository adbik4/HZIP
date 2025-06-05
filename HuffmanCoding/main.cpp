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
	File* message = File::getInstance("test.huf");
	bitVector bitstream = File::compress();
	std::cout << message->getMapping() << "\n";

	//std::cout << "decompression result : " << File::decompress(bitstream) << "\n";

	//std::vector<char> fl_tree = { 'A', 'B', 'C', 'D', 'E', 'F', 'G'};
	//HuffmanTree tree(fl_tree);

	auto tree = message->getTree();

	bitVector mask;
	std::vector<char> tree_data;
	std::tie(mask, tree_data) = tree->flatten();

	std::cout << "mask: " << mask << '\n';
	for (char x : tree_data) {
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