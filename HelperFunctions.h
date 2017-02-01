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

void validateRegex(string re)
{
	for (int i = 0; i < re.size(); i++)
	{
		if (int(re[i]) < 33 || int(re[i]) > 126)
		{
			std::cerr << "Unallowed char " << re[i] << " used in regex!\n";
			system("pause");
			exit(EXIT_FAILURE);
		}
	}
}

//if we are provided with a path as: D:\Users\Desktop append \ to make it valid
string constructFileName(dirent *directoryEntry, string path)
{
	string fileName = directoryEntry->d_name;

	int i = 0;
	while (path[i] != NULL) ++i;
	if (path[i - 1] != '\\') path += '\\';

	fileName = path + fileName;

	return fileName;
}

void replaceAll(string& str, const string& from, const string& to) 
{
	if (from.empty()) return;

	size_t start_pos = 0;

	while ((start_pos = str.find(from, start_pos)) != string::npos) 
	{
		str.replace(start_pos, from.length(), to);
		start_pos += to.length(); // In case the "to" string contains the "from" string
	}
}

void preprocessRegex(string &str, bool caseSensitive)
{
	//transform regex to lowercase if regex match is case insensitive
	if (!caseSensitive) transform(str.begin(), str.end(), str.begin(), tolower);

	//"\a" from stdin is represented as \\ + a in the string, so I convert it to a single special char \a (and \b, \f, \r)
	replaceAll(str, "(\\a)", "..\a");
	replaceAll(str, "(\\d)", "..\b");
	replaceAll(str, "(\\s)", "..\f");
	replaceAll(str, "(\\e)", "");
}

void readFile(string fileName, RegexParser regexMatcher, bool regexIsEmpty, bool caseSensitive)
{
	ifstream file(fileName);
	string line;
	int numberLine = 1;

	while (std::getline(file, line))
	{
		if (!caseSensitive) transform(line.begin(), line.end(), line.begin(), tolower);
		if (regexMatcher.match(0, line) || regexIsEmpty)
		{
			cout << fileName << ":" << numberLine << ":" << line << endl;
		}

		++numberLine;
	}
}

//we must have filename/directory and regex string provided from cmd line
void validateCmdParams(int argc, char* argv[])
{
	if (argc != 3)
	{
		std::cerr << "Usage: " << argv[0] << " <FILE or DIRECTORY> <REGEX>" << "\n";
		system("pause");
		exit(EXIT_FAILURE);
	}
}