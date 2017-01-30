#include<iostream>
#include<vector>
#include<string>
#include<stack>
#include<iterator>
#include "DynamicStack.h"

using namespace std;

struct transition 
{
	int vertex_from;
	int vertex_to;
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

	specialVert(): arrSize(0){}
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

	RegexParser concatenation(RegexParser a, RegexParser b);
	RegexParser orSelection(vector<RegexParser> selections, int no_of_selections);
	RegexParser iteration(RegexParser a);

	void Init();
	void setVertex(int numVertex);
	void setTransition(int vertex_from, int vertex_to, char transitionSymbol);
	void setFinalState(int fs);
	int getVertexCount();
	int getFinalState();
};










