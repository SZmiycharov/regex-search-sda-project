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
	//~RegexParser();
	//RegexParser(RegexParser const & obj);
	//RegexParser& operator=(RegexParser const & obj);

	RegexParser buildNFA(string re);
	bool match(int currentVertex, string remainingWord);
	void display();

	//bool RegexParser::match(string str);
private:
	vert vertex[100];
	transition transitions[100];
	specialVert specialVertexes[100];
	int sizeVertex = 0;
	int lengthSpecialVertexes;
	int lengthTransitions;
	int finalState;

	RegexParser concat(RegexParser a, RegexParser b);
	RegexParser or_selection(vector<RegexParser> selections, int no_of_selections);
	RegexParser kleene(RegexParser a);

	void Init();
	//void CopyFrom(RegexParser const& obj);
	void set_vertex(int numVertex);
	void set_transition(int vertex_from, int vertex_to, char transitionSymbol);
	void set_final_state(int fs);
	int get_vertex_count();
	int get_final_state();
};










