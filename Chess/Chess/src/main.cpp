#include <iostream>
#include <Windows.h>
#include "application.h"

int main(void)
{
	TCHAR gamePath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, gamePath);
	std::cout << gamePath << std::endl;
    Application::start();
}
