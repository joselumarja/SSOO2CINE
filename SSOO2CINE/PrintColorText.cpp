#include "pch.h"
#include "PrintColorText.h"

PrintColorText::PrintColorText()
{
}

void PrintColorText::printColorText(std::string Text, ColorText Color)
{
	std::lock_guard<std::shared_mutex> lk(mutex);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (int)Color);
	std::cout << Text << std::endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (int)ColorText::WhiteTextColor);
}

void PrintColorText::printErrorColorText(std::string Text, ColorText Color)
{
	std::lock_guard<std::shared_mutex> lk(mutex);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (int)Color);
	std::cerr << Text << std::endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (int)ColorText::WhiteTextColor);
}


