#include <string>
#include <vector>


#ifndef UTILITIES_H
#define UTILITIES_H
	const std::vector<wchar_t> aAccents = { L'á' ,L'à' ,L'ä',L'â',L'ã',L'ã',L'å'};
	const std::vector<wchar_t> eAccents = { L'é', L'è' ,L'ë' ,L'ê'};
	const std::vector<wchar_t> iAccents = { L'í' ,L'ì' ,L'ï' ,L'î'};
	const std::vector<wchar_t> oAccents = { L'ó' ,L'ò' ,L'ö' ,L'ô'};
	const std::vector<wchar_t> uAccents = { L'ú' ,L'ù' ,L'ü',L'û'};
	const std::vector<wchar_t> yAccents = { L'ý' ,L'ÿ'};

	const std::vector<wchar_t> AAccents = { L'Á' ,L'À' ,L'Ä' ,L'Â',L'Ã',L'Å'};
	const std::vector<wchar_t> EAccents = { L'É', L'È', L'Ë', L'Ê'};
	const std::vector<wchar_t> IAccents = { L'Í',L'Ì',L'Ï',L'Î'};
	const std::vector<wchar_t> OAccents = { L'Ó', L'Ò' ,L'Ö', L'Ô'};
	const std::vector<wchar_t> UAccents = {L'Ú',L'Ù',L'Ü',L'Û'};
	const std::vector<wchar_t> YAccents = {L'Ý'};

	unsigned int countFileLines(std::string FilePath);
	void seekFileLine(std::wifstream &File, unsigned int LineNumber);

	bool isTheSameCharacterWithAccent(wchar_t Character1, wchar_t Character2);
	bool isTheSameCharacterWithAccentImplicitDeclaration(wchar_t Character, wchar_t CharacterWithAccent);
	bool checkIfCharIsInList(wchar_t Character, std::vector<wchar_t> CharacterList);
#endif