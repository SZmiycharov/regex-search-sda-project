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
#include<vector>
#include<string>
#include<stack>
#include<iterator>
#include "DynamicStack.h"

using namespace std;

class RegexParser
{
public:
	RegexParser();
	/*we do not need the BIG 4 as nothing in our class is dynamically allocated (on the heap)*/

	RegexParser buildNFA(string re);
	bool match(int currentVertex, string remainingWord);
	void display();

private:
	struct transition
	{
		int vertexFrom;
		int vertexTo;
		char transitionSymbol;
	};

	struct vert
	{
		int indexString;
		int id;
	};

	struct specialVert
	{
		//by definition, every vertex can have 2 next vertexes or less
		int nextVert[3];
		char transitionSymbol[3];
		int arrSize;

		specialVert() : arrSize(0){}
	};

	vert vertex[40];
	transition transitions[100];
	specialVert specialVertexes[100];
	int sizeVertex = 0;
	int lengthSpecialVertexes;
	int lengthTransitions;
	int finalState;

	RegexParser concatenation(RegexParser op1, RegexParser op2);
	RegexParser orSelection(RegexParser selections[50], int numbSelections);
	RegexParser iteration(RegexParser a);

	void Init();
	void setVertex(int numVertex);
	void setTransition(int vertexFrom, int vertexTo, char transitionSymbol);
	void setFinalState(int finState);
	int getVertexCount();
	int getFinalState();
	static bool conditionEqual(string str, char ch);
};









