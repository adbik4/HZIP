#include <Windows.h>
#include "message.h"

int main()
{
	SetConsoleOutputCP(1250);
	Message msg("Hello World!");

	printProbability(msg.GetProbabilities());
	return 0;
}
