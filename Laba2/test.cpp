#include <conio.h>
#include <Windows.h>
#include "shannon_fano.h"

void draw(const std::string &string)
{
	const HANDLE descriptor{ GetStdHandle(STD_OUTPUT_HANDLE) };
	SetConsoleTextAttribute(descriptor, 14);
	std::cout << string << std::endl;
	SetConsoleTextAttribute(descriptor, 7);
}

void fall_back()
{
	std::cout << std::endl;
	draw(">Назад");
	int code{};
	do { code = _getch(); } while (code != 13);
}

int menu()
{
	int key = 0, code = 0;
	do {
		system("cls");
		key = (key + 2) % 2;
		if (key == 0) { draw(">Ввести строку"); }
		else { std::cout << "Ввести строку\n"; }
		if (key == 1) { draw(">Выйти"); }
		else { std::cout << "Выйти\n"; }
		if (code != 13)
		{
			code = _getch();
			if (code == 80 || code == 115) { ++key; }
			if (code == 72 || code == 119) { --key; }
		}
	} while (code != 13);
	system("cls");
	return key;
}

int main()
{
	setlocale(0, "");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	bool condition{ true };
	shannon_fano device{};
	while (condition)
	{
		const int option{ menu() };
		switch (option)
		{
		case 0:
		{
			std::string input{};
			std::cout << "Input a string:\t";
			getline(std::cin, input);
			device.encode(input);
			device.info();
			fall_back();
			break;
		}
		case 1:
		{
			condition = false;
			break;
		}
		}
	}
	return 0;
}
