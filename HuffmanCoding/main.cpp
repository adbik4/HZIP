#include <Windows.h>
#include <iomanip>

#include "file.h"
#include "tree.h"

File* File::instance = nullptr;

int main()
{
	SetConsoleOutputCP(1250);
	std::cout << std::setprecision(2);

	File* message = File::getInstance("A_DEAD_DAD_CEDED_A_BAD_BABE_A_BEADED_ABACA_BED");
	std::cout << message->GetMapping() << "\n";

	return 0;
}