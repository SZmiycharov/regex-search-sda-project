#include "HelperFunctions.h"

int main(int argc, char* argv[])
{
	//make sure we have file and regex provided
	validateCmdParams(argc, argv);

	RegexParser regexMatcher;

	//needed for getting all directory files
	DIR *dir;
	struct dirent *directoryEntry;

	//argv 1 is the given file name or path from console
	string path = argv[1];
	string regex = argv[2];

	cout << "regex before replace: " << regex << endl;

	preprocessRegex(regex);

	cout << "regex after replace: " << regex << endl;

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
		readFile(path, regexMatcher, regexIsEmpty);
	}
	else if ((dir = opendir(path.c_str())) != NULL) 
	{
		while ((directoryEntry = readdir(dir)) != NULL) 
		{
			string fileName = constructFileName(directoryEntry, path);

			readFile(fileName, regexMatcher, regexIsEmpty);
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