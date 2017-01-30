#include "RegexParser.h"

int RegexParser::get_vertex_count() {
	return sizeVertex;
}

void RegexParser::set_vertex(int no_vertex) {
	for (int i = 0; i < no_vertex; i++) {
		vertex[sizeVertex].id = i;
		sizeVertex++;
	}
}

void RegexParser::set_transition(int vertex_from, int vertex_to, char trans_symbol) {
	trans new_trans;
	new_trans.vertex_from = vertex_from;
	new_trans.vertex_to = vertex_to;
	new_trans.trans_symbol = trans_symbol;
	transitions[lengthTransitions] = new_trans;
	lengthTransitions++;
}

void RegexParser::set_final_state(int fs) {
	final_state = fs;
}

int RegexParser::get_final_state() {
	return final_state;
}

void RegexParser::display() {
	trans new_trans;
	//cout << "\n";
	for (int i = 0; i < lengthTransitions; i++) {
		new_trans = transitions[i];
		//cout << "q" << new_trans.vertex_from << " --> q" << new_trans.vertex_to << " : Symbol - " << new_trans.trans_symbol << endl;
	}
	//cout << "\nThe final state is q" << get_final_state() << endl;
}

bool RegexParser::actualMatch(int currentVertex, string remainingWord)
{
	//cout << "vertex: " << currentVertex << "; word: " << remainingWord << endl;
	if (remainingWord == "" && currentVertex == get_final_state()) return true;

	for (int i = 0; i < specialVertexes[currentVertex].arrSize; i++)
	{
		if (remainingWord != "" && specialVertexes[currentVertex].trans_symbol[i] == remainingWord[0])
		{
			if (actualMatch(specialVertexes[currentVertex].nextVert[i], remainingWord.substr(1, string::npos))) return true;
		}
		else if (specialVertexes[currentVertex].trans_symbol[i] == '^')
		{
			if (actualMatch(specialVertexes[currentVertex].nextVert[i], remainingWord)) return true;
		}
	}

	return false;
}

bool RegexParser::match(string str)
{
	int strCurIndex = 0;
	int i = 0;

	while (transitions[i].vertex_from > -1)
	{
		vertex[transitions[i].vertex_from].indexString = strCurIndex;
		/*cout << "q" << transitions[i].vertex_from << " --> q" 
			<< transitions[i].vertex_to << " : Symbol - " 
			<< transitions[i].trans_symbol << endl;*/

		if (transitions[i].trans_symbol == '^') 
		{
			//do nothing
		}
		else if (transitions[i].trans_symbol == str[strCurIndex])
		{
			++strCurIndex;
		}
		
		if (transitions[i].vertex_to == get_final_state())
		{
			if (str[strCurIndex] == NULL)
			{
				//cout << "MATCH BRO!!!" << endl;
				return true;
			}
			else
			{
				if (transitions[i + 1].vertex_from > -1)
				{
					strCurIndex = vertex[transitions[i + 1].vertex_from].indexString;
				}
				else
				{
					return false;
				}
			}
		}

		++i;
	}

	//cout << "\nThe final state is q" << get_final_state() << endl;

	return true;
}

RegexParser RegexParser::re_to_RegexParser(string re) {
	RegexParser test;
	DynamicStack<char> operators;
	DynamicStack<RegexParser> operands;
	char op_sym;
	int op_count;
	char cur_sym;
	RegexParser *new_sym;

	for (string::iterator it = re.begin(); it != re.end(); ++it) {
		cur_sym = *it;
		if (cur_sym != '(' && cur_sym != ')' && cur_sym != '*' && cur_sym != '|' && cur_sym != '.') {
			new_sym = new RegexParser();
			new_sym->set_vertex(2);
			new_sym->set_transition(0, 1, cur_sym);
			new_sym->set_final_state(1);
			operands.Push(*new_sym);
			delete new_sym;
		}
		else {
			if (cur_sym == '*') {
				RegexParser star_sym = operands.Top();
				operands.Pop();
				operands.Push(kleene(star_sym));
			}
			else if (cur_sym == '.') {
				operators.Push(cur_sym);
			}
			else if (cur_sym == '|') {
				operators.Push(cur_sym);
			}
			else if (cur_sym == '(') {
				operators.Push(cur_sym);
			}
			else {
				op_count = 0;
				char c;
				op_sym = operators.Top();
				if (op_sym == '(') continue;
				do {
					operators.Pop();
					op_count++;
				} while (operators.Top() != '(');
				operators.Pop();
				RegexParser op1;
				RegexParser op2;
				vector<RegexParser> selections;
				if (op_sym == '.') {
					for (int i = 0; i < op_count; i++) {
						op2 = operands.Top();
						operands.Pop();
						op1 = operands.Top();
						operands.Pop();
						operands.Push(concat(op1, op2));
					}
				}
				else if (op_sym == '|'){
					selections.assign(op_count + 1, RegexParser());
					int tracker = op_count;
					for (int i = 0; i < op_count + 1; i++) {
						selections.at(tracker) = operands.Top();
						tracker--;
						operands.Pop();
					}
					operands.Push(or_selection(selections, op_count + 1));
				}
				else {

				}
			}
		}
	}


	for (int i = 0; i < operands.Top().lengthTransitions; i++)
	{
		int specVertSize = operands.Top().specialVertexes[operands.Top().transitions[i].vertex_from].arrSize;
		int vertexFrom = operands.Top().transitions[i].vertex_from;
		
		if (specVertSize <= -800000000)
		{
			specVertSize = 0;
		}

		if (specialVertexes[vertexFrom].arrSize <= -800000000)
		{
			operands.Top().specialVertexes[vertexFrom].arrSize = 0;
			specialVertexes[vertexFrom].arrSize = 0;
		}

		//cout << operands.Top().transitions[i].vertex_to << " ";
		//cout << specialVertexes[vertexFrom].nextVert[specVertSize] << " ";
		operands.Top().specialVertexes[vertexFrom].nextVert[specVertSize] = operands.Top().transitions[i].vertex_to;
		operands.Top().specialVertexes[vertexFrom].trans_symbol[specVertSize] = operands.Top().transitions[i].trans_symbol;
		operands.Top().specialVertexes[vertexFrom].arrSize++;

		specialVertexes[vertexFrom].nextVert[specVertSize] = operands.Top().transitions[i].vertex_to;
		specialVertexes[vertexFrom].trans_symbol[specVertSize] = operands.Top().transitions[i].trans_symbol;
		specialVertexes[vertexFrom].arrSize++;
	}

	while (specialVertexes[lengthSpecialVertexes].arrSize > 0)
	{
		lengthSpecialVertexes++;
	}

	operands.Top().lengthSpecialVertexes = lengthSpecialVertexes;

	return operands.Top();
}

RegexParser RegexParser::concat(RegexParser a, RegexParser b) {
	RegexParser result;
	result.set_vertex(a.get_vertex_count() + b.get_vertex_count());
	int i;
	trans new_trans;

	for (i = 0; i < a.lengthTransitions; i++) {
		new_trans = a.transitions[i];
		result.set_transition(new_trans.vertex_from, new_trans.vertex_to, new_trans.trans_symbol);
	}

	result.set_transition(a.get_final_state(), a.get_vertex_count(), '^');

	for (i = 0; i < b.lengthTransitions; i++) {
		new_trans = b.transitions[i];
		result.set_transition(new_trans.vertex_from + a.get_vertex_count(), new_trans.vertex_to + a.get_vertex_count(), new_trans.trans_symbol);
	}

	result.set_final_state(a.get_vertex_count() + b.get_vertex_count() - 1);

	return result;
}

RegexParser RegexParser::kleene(RegexParser a) {
	RegexParser result;
	int i;
	trans new_trans;

	result.set_vertex(a.get_vertex_count() + 2);

	result.set_transition(0, 1, '^');

	for (i = 0; i < a.lengthTransitions; i++) {
		new_trans = a.transitions[i];
		result.set_transition(new_trans.vertex_from + 1, new_trans.vertex_to + 1, new_trans.trans_symbol);
	}

	result.set_transition(a.get_vertex_count(), a.get_vertex_count() + 1, '^');
	result.set_transition(a.get_vertex_count(), 1, '^');
	result.set_transition(0, a.get_vertex_count() + 1, '^');

	result.set_final_state(a.get_vertex_count() + 1);

	return result;
}

RegexParser RegexParser::or_selection(vector<RegexParser> selections, int no_of_selections) {
	RegexParser result;
	int vertex_count = 2;
	int i, j;
	RegexParser med;
	trans new_trans;

	for (i = 0; i < no_of_selections; i++) {
		vertex_count += selections.at(i).get_vertex_count();
	}

	result.set_vertex(vertex_count);

	int adder_track = 1;

	for (i = 0; i < no_of_selections; i++) {
		result.set_transition(0, adder_track, '^');
		med = selections.at(i);
		for (j = 0; j < med.lengthTransitions; j++) {
			new_trans = med.transitions[j];
			lengthTransitions++;
			result.set_transition(new_trans.vertex_from + adder_track, new_trans.vertex_to + adder_track, new_trans.trans_symbol);
		}
		adder_track += med.get_vertex_count();

		result.set_transition(adder_track - 1, vertex_count - 1, '^');
	}

	result.set_final_state(vertex_count - 1);

	return result;
}