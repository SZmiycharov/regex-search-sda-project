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
	int indexString;
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
	//vector<int> vertex;
	int vertex[50];
	int sizeVertex = 0;
	//vector<trans> transitions;
	trans transitions[50];
	int lengthTransitions = 0;
	int get_final_state();
	int get_vertex_count();
	void set_vertex(int no_vertex);
	void set_transition(int vertex_from, int vertex_to, char trans_symbol);
	void set_final_state(int fs);
	NFA concat(NFA a, NFA b);

	int final_state;

};










