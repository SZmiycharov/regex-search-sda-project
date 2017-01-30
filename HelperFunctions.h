/**
*
* Solution to homework task
* Data Structures Course
* Faculty of Mathematics and Informatics of Sofia University
* Winter semester 2016/2017
*
* @author Stanislav Zmiycharov
* @idnumber 61883
* @task 0
* @compiler VC
*
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <assert.h>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include <string>
#include "dirent.h"
#include "RegexParser.h"

void replaceSpecialCharactersRegex(string &str)
{
	size_t index = 0;
	while (true) {
		index = str.find("\\d", index);
		if (index == std::string::npos) break;

		str.replace(index, 2, "0|1|2|3|4|5|6|7|8|9");

		index += 2;
	}

	index = 0;

	while (true) {
		index = str.find("\\s", index);
		if (index == std::string::npos) break;

		str.replace(index, 2, " |	|\n");

		index += 2;
	}

	index = 0;

	while (true) {
		index = str.find("\\e", index);
		if (index == std::string::npos) break;

		str.replace(index, 2, "");

		index += 2;
	}

	index = 0;

	while (true) {
		index = str.find("\\a", index);
		if (index == std::string::npos) break;

		str.replace(index, 2, "a|b|c|d|e|f|g|h|i|j|k|l|m|n|o|p|q|r|s|t|u|v|w|x|y|z");

		index += 2;
	}
}

void readFile(string fileName, NFA regexMatcher, bool regexIsEmpty)
{
	ifstream file(fileName);
	string line;
	int numberLine = 0;

	while (std::getline(file, line))
	{
		if (regexMatcher.actualMatch(0, line) || regexIsEmpty)
		{
			cout << fileName << ":" << numberLine << ":" << line << endl;
		}

		++numberLine;
	}
}

//check if a number is given from console
void validateCmdParams(int argc, char* argv[])
{
	if (argc != 3)
	{
		std::cerr << "Usage: " << argv[0] << " <FILE or DIRECTORY> <REGEX>" << "\n";
		system("pause");
		exit(EXIT_FAILURE);
	}
}

//fill array with entries from binary file
void fileToArray(std::string fileName, uint64_t(&numbersFromFile)[10000], int &arrSize)
{
	std::fstream readFile;

	uint64_t num;

	readFile.open(fileName, std::ios_base::binary | std::ios_base::in);
	if (readFile.is_open())
	{
		while (readFile.read((char*)&num, sizeof(num)))
		{
			numbersFromFile[arrSize] = num;
			++arrSize;
		}
	}
	else
	{
		std::cerr << "Unable to open file " << fileName << "\n";
		exit(EXIT_FAILURE);
	}
	readFile.close();
}