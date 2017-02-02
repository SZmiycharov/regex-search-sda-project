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

/*******************************************
**This program allows you to match a regular expression string to lines in a file or multiple files in directory
**You should provide file name or directory name as second parameter on the console line
**You should also provide as third parameter the regular expression string
**The program understands these 4 special characters: \s = whitespace; \d = digit; \a = letter from English alphabet; \e = empty word
**Special symbols ("\s", "\d", "\a", "\e") should be enclosed in brackets "(f(\s))"
**Concatenation between char and special symbol must be implicit ("(f(\s))" instead of "(f.(\s))")
**Examples: 
** - ((ab)*|(cd)*) matches "" or "ababab" or "cdcdcd" etc.
** - (a*.b*) matches "" or "aaa" or "bbb" or "aaabbbb" etc.
** - (((a.b)|(c.d)).((f.x)|(z.h))) matches "abfx" or "abzh" or "cdfx" etc.
** - (b.a*) matches "b" or "ba" or "baaaa" etc
**Regex match is case insensitive by default - to make it case sensitive, specify the "-s" switch 
**		as fourth parameter when running the program
*******************************************/

#include "HelperFunctions.h"
#include <filesystem>
#include <sys/types.h>
#include <sys/stat.h>
using namespace std::tr2::sys;

int main(int argc, char* argv[])
{
	//make sure we have file and regex provided
	validateCmdParams(argc, argv);

	RegexParser regexMatcher;
	struct stat info;

	//argv 1 is the given file name or path from console
	string path = argv[1];
	string regex = argv[2];

	bool caseSensitive = false;
	if (argc == 4 && strcmp(argv[3], "-s") == 0)
	{
		caseSensitive = true;
	}

	validateRegex(regex);

	preprocessRegex(regex, caseSensitive);

	cout << "regex after process: " << regex << endl;

	bool regexIsEmpty = false;

	if (!regex.empty())
	{
		regexMatcher = regexMatcher.buildNFA(regex);
	}
	else
	{
		//the regex matches all lines in file if it is empty
		regexIsEmpty = true;
	}

	string line;
	int numberLine = 0;
	ifstream file(path);

	if (file.is_open())
	{
		readFile(path, regexMatcher, regexIsEmpty, caseSensitive);
	}
	else if (stat(path.c_str(), &info) != 0)
	{
		cerr << "Unable to open " << path << "\n";
		exit(EXIT_FAILURE);
	}
	else
	{
		for (recursive_directory_iterator i(path), end; i != end; ++i)
		{
			if (!is_directory(i->path()))
			{
				string fileName = i->path();

				readFile(fileName, regexMatcher, regexIsEmpty, caseSensitive);
			}
		}
	}

	system("pause");
	return 0;
}