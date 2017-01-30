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

void replaceAll(string& str, const string& from, const string& to) 
{
	if (from.empty()) return;

	size_t start_pos = 0;

	while ((start_pos = str.find(from, start_pos)) != string::npos) 
	{
		str.replace(start_pos, from.length(), to);
		start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
	}
}

void preprocessRegex(string &str)
{
	//transform regex to lowercase
	transform(str.begin(), str.end(), str.begin(), tolower);

	//replace all double backslashes with single ones
	replaceAll(str, "\\\\", "\\");

	//\d replaces all digits
	replaceAll(str, "(\\d)", "(0|1|2|3|4|5|6|7|8|9)");

	// \s replaces empty spaces
	replaceAll(str, "(\\s)", "( |	|\n)");

	//\e replaces the empty word
	replaceAll(str, ".(\\e)", "");

	//\d replaces all leters from alphabet
	replaceAll(str, "(\\a)", "(a|b|c|d|e|f|g|h|i|j|k|l|m|n|o|p|q|r|s|t|u|v|w|x|y|z)");
}

void readFile(string fileName, RegexParser regexMatcher, bool regexIsEmpty)
{
	ifstream file(fileName);
	string line;
	int numberLine = 1;

	while (std::getline(file, line))
	{
		transform(line.begin(), line.end(), line.begin(), tolower);
		if (regexMatcher.match(0, line) || regexIsEmpty)
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