#include "pch.h"
#include "Utilities.h"
#include <fstream>
#include <iostream>

unsigned int countFileLines(std::string FilePath)
{
	std::ifstream File;
	unsigned int LinesCounter = 0;
	std::string Buffer;

	File.open(FilePath, std::ios::in);
	
	if (!File.is_open())
	{
		std::cerr << "Fail opening the file: " << FilePath << std::endl;
		exit(EXIT_FAILURE);
	}

	while (!File.eof())
	{
		File.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		LinesCounter++;
	}

	File.close();

	return LinesCounter;
}

void seekFileLine(std::wifstream & File, unsigned int LineNumber)
{
	for (unsigned int i = 0; i < LineNumber; i++)
	{
		File.ignore(std::numeric_limits<std::streamsize>::max_digits10, '\n');
	}
}

bool isTheSameCharacterWithAccent(wchar_t Character1, wchar_t Character2)
{
	return isTheSameCharacterWithAccentImplicitDeclaration(Character1,Character2)||isTheSameCharacterWithAccentImplicitDeclaration(Character2,Character1);
}

bool isTheSameCharacterWithAccentImplicitDeclaration(wchar_t Character, wchar_t CharacterWithAccent)
{
	switch (Character)
	{
	case 'a':
		return checkIfCharIsInList(CharacterWithAccent, aAccents);
	case 'e':
		return checkIfCharIsInList(CharacterWithAccent, eAccents);
	case 'i':
		return checkIfCharIsInList(CharacterWithAccent, iAccents);
	case 'o':
		return checkIfCharIsInList(CharacterWithAccent, oAccents);
	case 'u':
		return checkIfCharIsInList(CharacterWithAccent, uAccents);
	case 'y':
		return checkIfCharIsInList(CharacterWithAccent, yAccents);
	case 'A':
		return checkIfCharIsInList(CharacterWithAccent, AAccents);
	case 'E':
		return checkIfCharIsInList(CharacterWithAccent, EAccents);
	case 'I':
		return checkIfCharIsInList(CharacterWithAccent, IAccents);
	case 'O':
		return checkIfCharIsInList(CharacterWithAccent, OAccents);
	case 'U':
		return checkIfCharIsInList(CharacterWithAccent, UAccents);
	case 'Y':
		return checkIfCharIsInList(CharacterWithAccent, YAccents);
	default:
		break;
	}
	return false;
}

bool checkIfCharIsInList(wchar_t Character, std::vector<wchar_t> CharacterList)
{
	for (wchar_t Ch : CharacterList)
	{
		if (Character == Ch)
		{
			return true;
		}

	}
	return false;
}
