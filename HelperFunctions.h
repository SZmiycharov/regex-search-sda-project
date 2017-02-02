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

//my regex parser relies that "." is used when concatenating, so I put "." between the symbols
//except they are special (like *, | etc.) when the user forgets to
void makeConcatExplicit(string &str)
{
	string helperStr = str;

	for (int i = str.size() - 1; i > 0; --i)
	{
		if (int(str[i]) != 92 && int(str[i]) != 40 && int(str[i]) != 41 && int(str[i]) != 42 && int(str[i]) != 46
			&& int(str[i]) != 124 && int(str[i-1]) != 124 && int(str[i - 1]) != 92 && int(str[i - 1]) != 40 && int(str[i - 1]) != 41 && int(str[i - 1]) != 46)
		{
			helperStr.insert(i, ".");
		}
	}

	str = helperStr;
}

void removeLastSlash(string &path)
{
	if (path[path.size() - 1] == '\\')
	{
		path = path.substr(0, path.size() - 1);
	}
}

//helper function to replace all occurences of from with to in string str
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

// the given regex MUST be into braces and quotes, so if the user does not provide them - I provide them
void ensureFormatting(string &str)
{
 	if (str[0] != '(' || str[str.size() - 1] != ')')
	{
		str.insert(0, "(");
		str.insert(str.size(), ")");
	}
}

void preprocessRegex(string &str, bool caseSensitive)
{
	ensureFormatting(str);
	makeConcatExplicit(str);
	
	//transform regex to lowercase if regex match is case insensitive
	if (!caseSensitive) transform(str.begin(), str.end(), str.begin(), tolower);

	//'\a' from stdin is represented as \\ + a in the string, so I convert it to a single special char '\a' (and '\b', '\f', '\r')
	replaceAll(str, "(\\a)", ".\a");
	replaceAll(str, "(\\d)", ".\b");
	replaceAll(str, "(\\s)", ".\f");
	replaceAll(str, "(\\e)", "");
}

void readFile(string fileName, RegexParser regexMatcher, bool regexIsEmpty, bool caseSensitive)
{
	ifstream file(fileName);
	string line;
	string lineOriginal;
	int numberLine = 1;

	while (std::getline(file, line))
	{
		lineOriginal = line;

		if (!caseSensitive) transform(line.begin(), line.end(), line.begin(), tolower);

		for (int i = 0; i < line.size(); i++)
		{
			if (regexMatcher.match(0, line.substr(i, string::npos)) || regexIsEmpty)
			{
				cout << fileName << ":" << numberLine << ":" << lineOriginal << endl;
				break;
			}
		}
		
		++numberLine;
	}
}

//we must have filename/directory and regex string provided from cmd line
void validateCmdParams(int argc, char* argv[])
{
	if (argc < 3)
	{
		std::cerr << "Usage: " << argv[0] << " <FILE or DIRECTORY> <REGEX>" << "\n";
		system("pause");
		exit(EXIT_FAILURE);
	}
}