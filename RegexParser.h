#include<iostream>
#include<vector>
#include<string>
#include<stack>
#include "DynamicStack.h"

using namespace std;

struct trans {
	int vertex_from;
	int vertex_to;
	char trans_symbol;
};

struct vert{
	int indexString;
	int id;
};

struct specialVert{
	int nextVert[50];
	char trans_symbol[50];
	int arrSize;

	specialVert(){
		arrSize = 0;
	}
};

class RegexParser {
public:
	RegexParser re_to_RegexParser(string re);
	
	RegexParser() {
		lengthTransitions = 0;
		//sizeVertex = 0;
		lengthSpecialVertexes = 0;
	}

	bool RegexParser::match(string str);
	
	void display();
	bool actualMatch(int currentVertex, string remainingWord);
	
private:
	RegexParser or_selection(vector<RegexParser> selections, int no_of_selections);
	RegexParser kleene(RegexParser a);
	vert vertex[100];
	int sizeVertex = 0;
	trans transitions[100];
	specialVert specialVertexes[100];
	int lengthSpecialVertexes;
	int lengthTransitions;
	int get_final_state();
	int get_vertex_count();
	void set_vertex(int no_vertex);
	void set_transition(int vertex_from, int vertex_to, char trans_symbol);
	void set_final_state(int fs);
	RegexParser concat(RegexParser a, RegexParser b);

	int final_state;

};










