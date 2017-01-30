#include "HelperFunctions.h"

int main(int argc, char* argv[])
{
	//make sure we have file and regex provided
	//validateCmdParams(argc, argv);

	RegexParser regexMatcher;

	//needed for getting all directory files
	DIR *dir;
	struct dirent *directoryEntry;

	//argv 1 is the given file name or path from console
	string path = "";
	string regex = "(a*)";

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

	RegexParser test = regexMatcher;

	cout << "yolo";
	system("pause");
	return 0;

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
			string fileName = directoryEntry->d_name;
			
			//append trailing slash to directory if it is not present
			int i = 0;
			while (path[i] != NULL) ++i;
			if (path[i - 1] != '\\') path += '\\';

			fileName = path + fileName;

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