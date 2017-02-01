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
**It MUST follow certain rules (or otherwise the program would not work properly):
** - it must be enclosed in quotes and brackets "(<regex>)"
** - the program understands these 4 special characters: \s = whitespace; \d = digit; \a = letter from English alphabet; \e = empty word
** - concatenation between chars should be shown explicitly ("(a.b)" instead of "(ab)")
** - special symbols ("\s", "\d", "\a", "\e") should be enclosed in brackets "(f(\s))"
** - concatenation between char and special symbol must be implicit ("(f(\s))" instead of "(f.(\s))")
** - ((a.b)*|(c.d)*) matches "" or "ababab" or "cdcdcd" etc.
** - (a*.b*) matches "" or "aaa" or "bbb" or "aaabbbb" etc.
** - (((a.b)|(c.d)).((f.x)|(z.h))) matches "abfx" or "abzh" or "cdfx" etc.
** - (b.a*) matches "b" or "ba" or "baaaa" etc
** - regex match is case insensitive by default - to make it case sensitive, specify the "-s" switch 
**		as fourth parameter when running the program
*******************************************/

#include "HelperFunctions.h"

int main(int argc, char* argv[])
{
	/*RegexParser test;
	test = test.buildNFA("(F.I.N.(\a))");

	cout << "fail" << endl;
	system("pause");
	return 0;*/

	//make sure we have file and regex provided
	//validateCmdParams(argc, argv);

	RegexParser regexMatcher;

	//needed for getting all directory files
	DIR *dir;
	struct dirent *directoryEntry;

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
	else if ((dir = opendir(path.c_str())) != NULL) 
	{
		while ((directoryEntry = readdir(dir)) != NULL) 
		{
			string fileName = constructFileName(directoryEntry, path);

			readFile(fileName, regexMatcher, regexIsEmpty, caseSensitive);
		}
		closedir(dir);
	}
	else 
	{
		cerr << "Unable to open " << path << "\n";
		exit(EXIT_FAILURE);
	}

	system("pause");
	return 0;
}