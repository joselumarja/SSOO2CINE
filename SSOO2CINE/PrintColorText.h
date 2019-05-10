#include <string>
#include <Windows.h>
#include <iostream>
#include <shared_mutex>


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

class PrintColorText
{

public:
	PrintColorText();
	void printColorText(std::string Text, ColorText Color);
	void printErrorColorText(std::string Text, ColorText Color);

private:
	std::shared_mutex mutex;
};
