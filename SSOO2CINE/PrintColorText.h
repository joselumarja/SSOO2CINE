#include <string>
#include <Windows.h>
#include <iostream>

#ifndef PRINTCOLORTEXT_H
#define PRINTCOLORTEXT_H

enum class ColorText
{
	DarkBlueTextColor = 9,
	GreenTextColor = 10,
	LightBlueTextColor = 11,
	RedTextColor = 12,
	PinkTextColor = 13,
	YellowTextColor = 14,
	WhiteTextColor = 15
};

void printColorText(std::string Text, ColorText Color);
void printErrorColorText(std::string Text, ColorText Color);

#endif