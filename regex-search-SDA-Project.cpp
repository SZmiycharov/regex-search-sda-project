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
#include "DynamicStack.h"

/*
* Convert infix regexp regexString to postfix notation.
* Insert . as explicit concatenation operator.
* Cheesy parser, return static buffer.
*/

int precedenceOf(char c)
{
	switch (c)
	{
	case '(': return 1;
	case '|': return 2;
	case '.': return 3;
	case '?': return 4;
	case '*': return 4;
	case '+': return 4;
	case '^': return 5;
	default: return 6;
	}
}

void regexInfixToPostfix(char *regexString, char* postRegex, int &lengthPostRegex)
{
	DynamicStack<char> stack;
	char result[20];
	int resLength = 0;
	
	for (int i = 0; regexString[i] != NULL; i++)
	{
		//current char
		char c = regexString[i];

		if (c == '(')
		{
			stack.Push(c);
		}
		else if (c == ')')
		{
			while (stack.Top() != '(')
			{
				postRegex[lengthPostRegex] = stack.Top();
				lengthPostRegex++;
				stack.Pop();
			}
			stack.Pop();
		}
		else
		{
			while (!stack.isEmpty())
			{
				char peekedChar = stack.Top();

				int peekedCharPrecedence = precedenceOf(peekedChar);
				int currentCharPrecedence = precedenceOf(c);

				if (peekedCharPrecedence >= currentCharPrecedence)
				{
					postRegex[lengthPostRegex] = stack.Top();
					lengthPostRegex++;
					stack.Pop();
				}
				else
				{
					break;
				}
			}
			stack.Push(c);
		}
	}

	while (!stack.isEmpty())
	{
		postRegex[lengthPostRegex] = stack.Top();
		lengthPostRegex++;
		stack.Pop();
	}
}

int main(int argc, char **argv)
{
	char postRegex[100];
	int lengthPostRegex = 0;
	char* beginningRegex = "((a.b|c.d).(w.x|y.z))";
	cout << beginningRegex << "----- beginning regex!" << endl;
	regexInfixToPostfix(beginningRegex, postRegex, lengthPostRegex);
	
	cout << "FINAL:\n";
	for (int i = 0; i < lengthPostRegex; i++)
	{
		cout << postRegex[i];
	}

	system("pause");
	return 0;
}

