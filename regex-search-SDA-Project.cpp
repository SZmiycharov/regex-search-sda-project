#include<iostream>
#include<vector>
#include<string>
#include<set>
#include<stack>
#include "RegexParser.h"
#include "HelperFunctions.h"

#include "dirent.h"


int main(int argc, char* argv[])
{
	NFA required_nfa;
	
	/*cout << "\n\nDISPLAY:";
	required_nfa.display();
	cout << "\n\n";
	required_nfa.match("aaaaaa");
	cout << "\nRESULT: " << required_nfa.actualMatch(0, "ab") << endl;*/


	DIR *dir;
	struct dirent *ent;

	validateCmdParams(argc, argv);
	
	cout << argv[1] << endl;
	string name = argv[1];

	//"(a.b)"
	required_nfa = required_nfa.re_to_nfa(argv[2]);

	//string name = "D:\\Users\\Desktop\\test\\";
	string str;
	int numberLine = 0;
	ifstream file(name);

	if (file.is_open())
	{
		cout << "reading file: " << name << endl;
		while (getline(file, str))
		{
			cout << numberLine << " : " << str << " ";
			cout << "\nRESULT: " << required_nfa.actualMatch(0, str) << endl;
			++numberLine;
		}
	}
	else if ((dir = opendir(name.c_str())) != NULL) 
	{
		while ((ent = readdir(dir)) != NULL) 
		{
			string x = ent->d_name;
			x = name + x;

			cout << "\nFILENAME: " << x << endl;

			ifstream file(x);
			numberLine = 0;
			while (std::getline(file, str))
			{
				cout << numberLine << " : " << str << " ";
				cout << "RESULT: " << required_nfa.actualMatch(0, str) << endl;
				++numberLine;
			}
		}
		closedir(dir);
	}
	else {
		/* could not open directory */
		cout << "FAIL" << endl;
	}

	system("pause");
	return 0;
}

//int main(int argc, char* argv[])
//{
//	validateCmdParams(argc, argv);
//
//	cout << argv[1] << endl;
//	char* fileName = argv[1];
//
//	ifstream file(fileName);
//	string str;
//
//	DIR *dir;
//	struct dirent *ent;
//	if ((dir = opendir("c:\\src\\")) != NULL) {
//		/* print all the files and directories within directory */
//		while ((ent = readdir(dir)) != NULL) {
//			printf("%s\n", ent->d_name);
//		}
//		closedir(dir);
//	}
//	else {
//		/* could not open directory */
//		perror("");
//		return EXIT_FAILURE;
//	}
//	
//	int numberLine = 0;
//	while (std::getline(file, str))
//	{
//		cout << str << " : " << numberLine << endl;
//		++numberLine;
//	}
//
//	//NFA required_nfa;
//	//required_nfa = required_nfa.re_to_nfa("(a.(b*))");
//	//cout << "\n\nDISPLAY:";
//	//required_nfa.display();
//	//cout << "\n\n";
//	////required_nfa.match("aaaaaa");
//	//cout << "\nRESULT: " << required_nfa.actualMatch(0, "ab") << endl;
//
//	system("pause");
//	return 0;
//}