#include "HelperFunctions.h"

int main(int argc, char* argv[])
{
//	string str;
//	cout << "enter text: ";
//	cin >> str;
//	cout << "string before: " << str << endl;
//	replaceSpecialCharactersRegex(str);
//	cout << "string after: " << str << endl;
//
//	system("pause");
//	return 0;
//
//
//
//	str = "(\a)***a.b.c.***(\s)***alsdk;lasd***(\s)***a;lsdjas(\s)";
//	//str.replace(9, 5, "a|b|c|d|f");
////	cout << "string before: " << str << endl;
//
//
//	replaceSpecialCharactersRegex(str);
//
//	//cout << "string now: " << str << endl;
//
//	system("pause");
//	return 0;

	//make sure we have file and regex provided
	validateCmdParams(argc, argv);

	NFA regexMatcher;

	//needed for getting all directory files
	DIR *dir;
	struct dirent *directoryEntry;

	//argv 1 is the given file name or path from console
	string path = argv[1];
	string regex = argv[2];

	bool regexIsEmpty = false;

	if (!regex.empty())
	{
		regexMatcher = regexMatcher.re_to_nfa(regex);
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