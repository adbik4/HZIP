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
	std::shared_ptr<File> message = File::getInstance("AAAAABBBCCD");
	message->writeFile("test_file.huf");
	std::cout << "DONE \n";

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