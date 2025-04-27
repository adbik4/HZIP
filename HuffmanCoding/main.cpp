#include <Windows.h>
#include <iomanip>

#include "file.h"
#include "tree.h"

File* File::instance = nullptr;

int main()
{
	SetConsoleOutputCP(1250);
	std::cout << std::setprecision(2);

	File* message = File::getInstance("Hello World!");
	(message->GetTree()).printPostOrder();

	return 0;
}