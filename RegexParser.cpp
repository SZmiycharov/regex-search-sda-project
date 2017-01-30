#include "RegexParser.h"

RegexParser::RegexParser() 
{
	Init();
}

//RegexParser::RegexParser(RegexParser const & obj)
//{
//	Init();
//	CopyFrom(obj);
//}
//
//RegexParser& RegexParser::operator=(RegexParser const & obj)
//{
//	if (this != &obj)
//	{
//		CopyFrom(obj);
//	}
//
//	return *this;
//}

RegexParser RegexParser::buildNFA(string re)
{
	RegexParser test;
	DynamicStack<char> operators;
	DynamicStack<RegexParser> operands;
	char op_sym;
	int op_count;
	char cur_sym;
	RegexParser *new_sym;

	for (string::iterator it = re.begin(); it != re.end(); ++it)
	{
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
			if (cur_sym == '*')
			{
				RegexParser star_sym = operands.Top();
				operands.Pop();
				operands.Push(kleene(star_sym));
			}
			else if (cur_sym == '.')
			{
				operators.Push(cur_sym);
			}
			else if (cur_sym == '|')
			{
				operators.Push(cur_sym);
			}
			else if (cur_sym == '(')
			{
				operators.Push(cur_sym);
			}
			else
			{
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
				if (op_sym == '.')
				{
					for (int i = 0; i < op_count; i++)
					{
						op2 = operands.Top();
						operands.Pop();
						op1 = operands.Top();
						operands.Pop();
						operands.Push(concat(op1, op2));
					}
				}
				else if (op_sym == '|')
				{
					selections.assign(op_count + 1, RegexParser());
					int tracker = op_count;
					for (int i = 0; i < op_count + 1; i++)
					{
						selections.at(tracker) = operands.Top();
						tracker--;
						operands.Pop();
					}
					operands.Push(or_selection(selections, op_count + 1));
				}
				else
				{

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
		operands.Top().specialVertexes[vertexFrom].transitionSymbol[specVertSize] = operands.Top().transitions[i].transitionSymbol;
		operands.Top().specialVertexes[vertexFrom].arrSize++;

		specialVertexes[vertexFrom].nextVert[specVertSize] = operands.Top().transitions[i].vertex_to;
		specialVertexes[vertexFrom].transitionSymbol[specVertSize] = operands.Top().transitions[i].transitionSymbol;
		specialVertexes[vertexFrom].arrSize++;
	}

	while (specialVertexes[lengthSpecialVertexes].arrSize > 0)
	{
		lengthSpecialVertexes++;
	}

	operands.Top().lengthSpecialVertexes = lengthSpecialVertexes;

	return operands.Top();
}

bool RegexParser::match(int currentVertex, string remainingWord)
{
	//cout << "vertex: " << currentVertex << "; word: " << remainingWord << endl;
	if (remainingWord == "" && currentVertex == get_final_state()) return true;

	for (int i = 0; i < specialVertexes[currentVertex].arrSize; i++)
	{
		if (remainingWord != "" && specialVertexes[currentVertex].transitionSymbol[i] == remainingWord[0])
		{
			if (match(specialVertexes[currentVertex].nextVert[i], remainingWord.substr(1, string::npos))) return true;
		}
		else if (specialVertexes[currentVertex].transitionSymbol[i] == '^')
		{
			if (match(specialVertexes[currentVertex].nextVert[i], remainingWord)) return true;
		}
	}

	return false;
}

void RegexParser::display()
{
	transition newTransition;
	//cout << "\n";
	for (int i = 0; i < lengthTransitions; i++)
	{
		newTransition = transitions[i];
		//cout << "q" << newTransition.vertex_from << " --> q" << newTransition.vertex_to << " : Symbol - " << newTransition.transitionSymbol << endl;
	}
	//cout << "\nThe final state is q" << get_final_state() << endl;
}

RegexParser RegexParser::concat(RegexParser a, RegexParser b)
{
	RegexParser result;
	result.set_vertex(a.get_vertex_count() + b.get_vertex_count());
	int i;
	transition newTransition;

	for (i = 0; i < a.lengthTransitions; i++)
	{
		newTransition = a.transitions[i];
		result.set_transition(newTransition.vertex_from, newTransition.vertex_to, newTransition.transitionSymbol);
	}

	result.set_transition(a.get_final_state(), a.get_vertex_count(), '^');

	for (i = 0; i < b.lengthTransitions; i++)
	{
		newTransition = b.transitions[i];
		result.set_transition(newTransition.vertex_from + a.get_vertex_count(), newTransition.vertex_to + a.get_vertex_count(), newTransition.transitionSymbol);
	}

	result.set_final_state(a.get_vertex_count() + b.get_vertex_count() - 1);

	return result;
}

RegexParser RegexParser::or_selection(vector<RegexParser> selections, int no_of_selections)
{
	RegexParser result;
	int vertex_count = 2;
	int i, j;
	RegexParser med;
	transition newTransition;

	for (i = 0; i < no_of_selections; i++) {
		vertex_count += selections.at(i).get_vertex_count();
	}

	result.set_vertex(vertex_count);

	int adder_track = 1;

	for (i = 0; i < no_of_selections; i++)
	{
		result.set_transition(0, adder_track, '^');
		med = selections.at(i);
		for (j = 0; j < med.lengthTransitions; j++)
		{
			newTransition = med.transitions[j];
			lengthTransitions++;
			result.set_transition(newTransition.vertex_from + adder_track, newTransition.vertex_to + adder_track, newTransition.transitionSymbol);
		}
		adder_track += med.get_vertex_count();

		result.set_transition(adder_track - 1, vertex_count - 1, '^');
	}

	result.set_final_state(vertex_count - 1);

	return result;
}

RegexParser RegexParser::kleene(RegexParser a) 
{
	RegexParser result;
	int i;
	transition newTransition;

	result.set_vertex(a.get_vertex_count() + 2);

	result.set_transition(0, 1, '^');

	for (i = 0; i < a.lengthTransitions; i++)
	{
		newTransition = a.transitions[i];
		result.set_transition(newTransition.vertex_from + 1, newTransition.vertex_to + 1, newTransition.transitionSymbol);
	}

	result.set_transition(a.get_vertex_count(), a.get_vertex_count() + 1, '^');
	result.set_transition(a.get_vertex_count(), 1, '^');
	result.set_transition(0, a.get_vertex_count() + 1, '^');

	result.set_final_state(a.get_vertex_count() + 1);

	return result;
}

void RegexParser::Init()
{
	lengthTransitions = 0;
	lengthSpecialVertexes = 0;
}

//void RegexParser::CopyFrom(RegexParser const& obj)
//{
//	if (obj.lengthSpecialVertexes == 0) return;
//
//	sizeVertex = obj.sizeVertex;
//	lengthSpecialVertexes = obj.lengthSpecialVertexes;
//	lengthTransitions = obj.lengthTransitions;
//	finalState = obj.finalState;
//
//	std::copy(std::begin(obj.vertex), std::end(obj.vertex), std::begin(vertex));
//	std::copy(std::begin(obj.transitions), std::end(obj.transitions), std::begin(transitions));
//	std::copy(std::begin(obj.specialVertexes), std::end(obj.specialVertexes), std::begin(specialVertexes));
//}

void RegexParser::set_vertex(int numVertex)
{
	for (int i = 0; i < numVertex; i++)
	{
		vertex[sizeVertex].id = i;
		sizeVertex++;
	}
}

void RegexParser::set_transition(int vertex_from, int vertex_to, char transitionSymbol)
{
	transition newTransition;
	newTransition.vertex_from = vertex_from;
	newTransition.vertex_to = vertex_to;
	newTransition.transitionSymbol = transitionSymbol;
	transitions[lengthTransitions] = newTransition;
	lengthTransitions++;
}

void RegexParser::set_final_state(int fs)
{
	finalState = fs;
}

int RegexParser::get_vertex_count() 
{
	return sizeVertex;
}

int RegexParser::get_final_state() 
{
	return finalState;
}

//bool RegexParser::match(string str)
//{
//	int strCurIndex = 0;
//	int i = 0;
//
//	while (transitions[i].vertex_from > -1)
//	{
//		vertex[transitions[i].vertex_from].indexString = strCurIndex;
//		/*cout << "q" << transitions[i].vertex_from << " --> q" 
//			<< transitions[i].vertex_to << " : Symbol - " 
//			<< transitions[i].transitionSymbol << endl;*/
//
//		if (transitions[i].transitionSymbol == '^') 
//		{
//			//do nothing
//		}
//		else if (transitions[i].transitionSymbol == str[strCurIndex])
//		{
//			++strCurIndex;
//		}
//		
//		if (transitions[i].vertex_to == get_final_state())
//		{
//			if (str[strCurIndex] == NULL)
//			{
//				//cout << "MATCH BRO!!!" << endl;
//				return true;
//			}
//			else
//			{
//				if (transitions[i + 1].vertex_from > -1)
//				{
//					strCurIndex = vertex[transitions[i + 1].vertex_from].indexString;
//				}
//				else
//				{
//					return false;
//				}
//			}
//		}
//
//		++i;
//	}
//
//	//cout << "\nThe final state is q" << get_final_state() << endl;
//
//	return true;
//}





