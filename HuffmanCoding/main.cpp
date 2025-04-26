#include <Windows.h>
#include "message.h"
#include "tree.h"
#include "storage.h"

File* File::instance = nullptr;

int main()
{
	SetConsoleOutputCP(1250);

	File* message = File::getInstance("Hello World!");
	std::cout << message->GetMapping() << "\n";

	return 0;
}