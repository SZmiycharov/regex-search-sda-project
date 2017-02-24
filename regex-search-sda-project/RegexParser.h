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

#include<iostream>
#include<string>
#include<iterator>
#include "DynamicStack.h"

using namespace std;

class RegexParser
{
public:
	RegexParser();
	/*we do not need the BIG 4 as nothing in our class is dynamically allocated (on the heap)*/

	RegexParser buildNFA(string regex);
	bool match(int currentVertex, string remainingWord);

private:
	struct transition
	{
		int vertexFrom;
		int vertexTo;
		char transitionSymbol;
	};

	struct vertex
	{
		//by definition (Thompson's algorithm), every vertex can have 2 next vertexes or less
		int nextVert[3];
		char transitionSymbol[3];
		int arrSize;

		vertex() : arrSize(0){}
	};

	transition transitions[500];
	vertex vertexes[500];
	int countVertexes;
	int lengthTransitions;
	int finalState;

	RegexParser concatenation(RegexParser op1, RegexParser op2);
	RegexParser orSelection(RegexParser selections[50], int numbSelections);
	RegexParser iteration(RegexParser operandSymb);

	void increaseVertexCount(int vertexCount);
	void setTransition(int vertexFrom, int vertexTo, char transitionSymbol);
	void setFinalState(int finState);
	int getFinalState();
	int getVertexCount();
	static bool conditionEqual(string str, char ch);
};









