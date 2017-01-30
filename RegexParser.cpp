#include "RegexParser.h"

RegexParser::RegexParser() 
{
	lengthTransitions = 0;
	lengthSpecialVertexes = 0;
}

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
			new_sym->setVertex(2);
			new_sym->setTransition(0, 1, cur_sym);
			new_sym->setFinalState(1);
			operands.Push(*new_sym);
			delete new_sym;
		}
		else {
			if (cur_sym == '*')
			{
				RegexParser star_sym = operands.Top();
				operands.Pop();
				operands.Push(iteration(star_sym));
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
						operands.Push(concatenation(op1, op2));
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
					operands.Push(orSelection(selections, op_count + 1));
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
	if (remainingWord == "" && currentVertex == getFinalState()) return true;

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
	//cout << "\nThe final state is q" << getFinalState() << endl;
}

RegexParser RegexParser::concatenation(RegexParser a, RegexParser b)
{
	RegexParser result;
	result.setVertex(a.getVertexCount() + b.getVertexCount());
	int i;
	transition newTransition;

	for (i = 0; i < a.lengthTransitions; i++)
	{
		newTransition = a.transitions[i];
		result.setTransition(newTransition.vertex_from, newTransition.vertex_to, newTransition.transitionSymbol);
	}

	result.setTransition(a.getFinalState(), a.getVertexCount(), '^');

	for (i = 0; i < b.lengthTransitions; i++)
	{
		newTransition = b.transitions[i];
		result.setTransition(newTransition.vertex_from + a.getVertexCount(), newTransition.vertex_to + a.getVertexCount(), newTransition.transitionSymbol);
	}

	result.setFinalState(a.getVertexCount() + b.getVertexCount() - 1);

	return result;
}

RegexParser RegexParser::orSelection(vector<RegexParser> selections, int no_of_selections)
{
	RegexParser result;
	int vertex_count = 2;
	int i, j;
	RegexParser med;
	transition newTransition;

	for (i = 0; i < no_of_selections; i++) {
		vertex_count += selections.at(i).getVertexCount();
	}

	result.setVertex(vertex_count);

	int adder_track = 1;

	for (i = 0; i < no_of_selections; i++)
	{
		result.setTransition(0, adder_track, '^');
		med = selections.at(i);
		for (j = 0; j < med.lengthTransitions; j++)
		{
			newTransition = med.transitions[j];
			lengthTransitions++;
			result.setTransition(newTransition.vertex_from + adder_track, newTransition.vertex_to + adder_track, newTransition.transitionSymbol);
		}
		adder_track += med.getVertexCount();

		result.setTransition(adder_track - 1, vertex_count - 1, '^');
	}

	result.setFinalState(vertex_count - 1);

	return result;
}

RegexParser RegexParser::iteration(RegexParser a) 
{
	RegexParser result;
	int i;
	transition newTransition;

	result.setVertex(a.getVertexCount() + 2);

	result.setTransition(0, 1, '^');

	for (i = 0; i < a.lengthTransitions; i++)
	{
		newTransition = a.transitions[i];
		result.setTransition(newTransition.vertex_from + 1, newTransition.vertex_to + 1, newTransition.transitionSymbol);
	}

	result.setTransition(a.getVertexCount(), a.getVertexCount() + 1, '^');
	result.setTransition(a.getVertexCount(), 1, '^');
	result.setTransition(0, a.getVertexCount() + 1, '^');

	result.setFinalState(a.getVertexCount() + 1);

	return result;
}

void RegexParser::setVertex(int numVertex)
{
	for (int i = 0; i < numVertex; i++)
	{
		vertex[sizeVertex].id = i;
		sizeVertex++;
	}
}

void RegexParser::setTransition(int vertex_from, int vertex_to, char transitionSymbol)
{
	transition newTransition;
	newTransition.vertex_from = vertex_from;
	newTransition.vertex_to = vertex_to;
	newTransition.transitionSymbol = transitionSymbol;
	transitions[lengthTransitions] = newTransition;
	lengthTransitions++;
}

void RegexParser::setFinalState(int fs)
{
	finalState = fs;
}

int RegexParser::getVertexCount() 
{
	return sizeVertex;
}

int RegexParser::getFinalState() 
{
	return finalState;
}