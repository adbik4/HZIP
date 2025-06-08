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
	std::shared_ptr<File> file = File::getInstance("test_file.txt");
	std::cout << file->getContents() << "\n";
	std::cout << file->getMapping() << '\n';

	//bitVector mask;
	//std::vector<char> data;
	//std::tie(mask, data) = file->getTree()->flatten();
	//std::cout << "mask: " << mask << '\n';
	//for (char c : data) {
	//	if (c == '\0') {
	//		std::cout << "[null]\n";
	//	}
	//	else if (c == ' ') {
	//		std::cout << "[space]\n";
	//	}
	//	else {
	//		std::cout << c << '\n';
	//	}
	//}

	//	 WHAT THE CODE SHOULD LOOK LIKE
	//	 decompress:
	//	File file = openFile("filepath.txt"); // automatically decompresses, reads tables and trees, fills in its properties
	//	std::cout << file.getContents();

	//	 compress:
	//	std::string content = "ABBCCC";
	//	File file(content); // fills in its properties, generates tables and trees
	//	file.writeFile("filepath.txt"); // automatically compresses

	return 0;
}