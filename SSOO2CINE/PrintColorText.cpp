#include "pch.h"
#include "PrintColorText.h"

void printColorText(std::string Text, ColorText Color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (int)Color);
	std::cout << Text << std::endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (int)ColorText::WhiteTextColor);
}

void printErrorColorText(std::string Text, ColorText Color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (int)Color);
	std::cerr << Text << std::endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (int)ColorText::WhiteTextColor);
}
