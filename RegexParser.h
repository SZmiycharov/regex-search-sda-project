#include<iostream>
#include<vector>
#include<string>
#include<stack>
#include<iterator>
#include "DynamicStack.h"

using namespace std;

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
	int nextVert[50];
	char transitionSymbol[50];
	int arrSize;

	specialVert() : arrSize(0){}
};

class RegexParser
{
public:
	RegexParser();
	/*we do not need the BIG 4 as nothing in our class is dynamically allocated (on the heap)*/

	RegexParser buildNFA(string re);
	bool match(int currentVertex, string remainingWord);
	void display();

private:
	vert vertex[100];
	transition transitions[100];
	specialVert specialVertexes[100];
	int sizeVertex = 0;
	int lengthSpecialVertexes;
	int lengthTransitions;
	int finalState;

	RegexParser concatenation(RegexParser op1, RegexParser op2);
	RegexParser orSelection(vector<RegexParser> selections, int numbSelections);
	RegexParser iteration(RegexParser a);

	void Init();
	void setVertex(int numVertex);
	void setTransition(int vertexFrom, int vertexTo, char transitionSymbol);
	void setFinalState(int finState);
	int getVertexCount();
	int getFinalState();
	static bool conditionCharEqual(string str, char ch);
};









