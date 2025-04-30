﻿#include <Windows.h>
#include <iomanip>

#include "file.h"
#include "tree.h"
#include "types.h"

File* File::instance = nullptr;
std::string File::_content;
std::unordered_map<char, Symbol> File::_huffMap;
HuffmanTree* File::_huffTree = nullptr;

int main()
{
	// init
	SetConsoleOutputCP(1250);
	std::cout << std::fixed << std::setprecision(3);

	// code 
	File* message = File::getInstance("Hello World!");
	bitVector bitstream = File::compress();
	std::cout << message->getMapping() << "\n";

	std::cout << "decompression result : " << File::decompress(bitstream) << "\n";

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