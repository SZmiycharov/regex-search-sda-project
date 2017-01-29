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

class NFA {
public:
	NFA re_to_nfa(string re);
	
	NFA() {
		lengthTransitions = 0;
		sizeVertex = 0;
	}

	bool NFA::match(string str);
	
	void display();
	

private:
	NFA or_selection(vector<NFA> selections, int no_of_selections);
	NFA kleene(NFA a);
	vert vertex[10];
	int sizeVertex = 0;
	trans transitions[12];
	specialVert specialVertexes[12];
	int lengthTransitions = 0;
	int get_final_state();
	int get_vertex_count();
	void set_vertex(int no_vertex);
	void set_transition(int vertex_from, int vertex_to, char trans_symbol);
	void set_final_state(int fs);
	NFA concat(NFA a, NFA b);

	int final_state;

};









