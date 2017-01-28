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

#include "HelperFunctions.h"
using namespace std;

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
* Convert infix regexp regexString to postfix notation.
* Insert . as explicit concatenation operator.
* Cheesy parser, return static buffer.
*/
void re2post(char *regexString, char* postRegex, int &lengthPostRegex)
{
	int nalternation, natom;

	struct {
		int nalternation;
		int natom;
	} parentheses[100], *p;

	p = parentheses;
	nalternation = 0;
	natom = 0;
	
	for (int i = 0; regexString[i] != NULL; i++)
	{
		switch (regexString[i])
		{
			case '(':
				if (natom > 1){
					--natom;
					postRegex[lengthPostRegex] = '.';
					++lengthPostRegex;
				}
				if (p >= parentheses + 100)
					return;
				p->nalternation = nalternation;
				p->natom = natom;
				p++;
				nalternation = 0;
				natom = 0;
				break;
			case '|':
				if (natom == 0)
					return;
				while (--natom > 0)
				{
					postRegex[lengthPostRegex] = '.';
					++lengthPostRegex;
				}
					
				nalternation++;
				break;
			case ')':
				if (p == parentheses)
					return;
				if (natom == 0)
					return;
				while (--natom > 0)
				{
					postRegex[lengthPostRegex] = '.';
					++lengthPostRegex;
				}
				for (; nalternation > 0; nalternation--)
				{
					postRegex[lengthPostRegex] = '|';
					++lengthPostRegex;
				}
					
				--p;
				nalternation = p->nalternation;
				natom = p->natom;
				natom++;
				break;
			case '*':
			case '+':
			case '?':
				if (natom == 0)
					return;
				postRegex[lengthPostRegex] = regexString[i];
				++lengthPostRegex;
				break;
			case '.':
				if (natom > 1){
					--natom;

					postRegex[lengthPostRegex] = '.';
					++lengthPostRegex;
				}
				natom++;
				break;
			default:
				if (natom > 1){
					--natom;
					postRegex[lengthPostRegex] = '.';
					++lengthPostRegex;
				}
				postRegex[lengthPostRegex] = regexString[i];
				++lengthPostRegex;

				natom++;
				break;
		}
	}
	if (p != parentheses)
		return;
	while (--natom > 0)
	{
		postRegex[lengthPostRegex] = '.';
		++lengthPostRegex;
	}
	for (; nalternation > 0; nalternation--)
	{
		postRegex[lengthPostRegex] = '|';
		++lengthPostRegex;
	}
}

int main(int argc, char **argv)
{
	char postRegex[100];
	int lengthPostRegex = 0;
	re2post("(a|s)*d", postRegex, lengthPostRegex);
	cout << "*****";
	for (int i = 0; i < lengthPostRegex; i++)
	{
		cout << postRegex[i];
	}
	cout << "****";


	system("pause");
	return 0;
}

