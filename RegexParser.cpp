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
		int specVertSize = operands.Top().specialVertexes[operands.Top().transitions[i].vertexFrom].arrSize;
		int vertexFrom = operands.Top().transitions[i].vertexFrom;

		if (specVertSize <= -1)
		{
			specVertSize = 0;
		}

		if (specialVertexes[vertexFrom].arrSize <= -1)
		{
			operands.Top().specialVertexes[vertexFrom].arrSize = 0;
			specialVertexes[vertexFrom].arrSize = 0;
		}

		//cout << operands.Top().transitions[i].vertexTo << " ";
		//cout << specialVertexes[vertexFrom].nextVert[specVertSize] << " ";
		operands.Top().specialVertexes[vertexFrom].nextVert[specVertSize] = operands.Top().transitions[i].vertexTo;
		operands.Top().specialVertexes[vertexFrom].transitionSymbol[specVertSize] = operands.Top().transitions[i].transitionSymbol;
		operands.Top().specialVertexes[vertexFrom].arrSize++;

		specialVertexes[vertexFrom].nextVert[specVertSize] = operands.Top().transitions[i].vertexTo;
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
		// (remainingWord != "" && specialVertexes[currentVertex].transitionSymbol[i] == remainingWord[0])
		if (RegexParser::conditionCharEqual(remainingWord, specialVertexes[currentVertex].transitionSymbol[i]))
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
		//cout << "q" << newTransition.vertexFrom << " --> q" << newTransition.vertexTo << " : Symbol - " << newTransition.transitionSymbol << endl;
	}
	//cout << "\nThe final state is q" << getFinalState() << endl;
}

RegexParser RegexParser::concatenation(RegexParser op1, RegexParser op2)
{
	RegexParser result;
	result.setVertex(op1.getVertexCount() + op2.getVertexCount());
	int i;
	transition newTransition;

	for (i = 0; i < op1.lengthTransitions; i++)
	{
		newTransition = op1.transitions[i];
		result.setTransition(newTransition.vertexFrom, newTransition.vertexTo, newTransition.transitionSymbol);
	}

	result.setTransition(op1.getFinalState(), op1.getVertexCount(), '^');

	for (i = 0; i < op2.lengthTransitions; i++)
	{
		newTransition = op2.transitions[i];
		result.setTransition(newTransition.vertexFrom + op1.getVertexCount(), newTransition.vertexTo + op1.getVertexCount(), newTransition.transitionSymbol);
	}

	result.setFinalState(op1.getVertexCount() + op2.getVertexCount() - 1);

	return result;
}

RegexParser RegexParser::orSelection(vector<RegexParser> selections, int numbSelections)
{
	RegexParser result;
	int vertex_count = 2;
	int i, j;
	RegexParser med;
	transition newTransition;

	for (i = 0; i < numbSelections; i++) {
		vertex_count += selections.at(i).getVertexCount();
	}

	result.setVertex(vertex_count);

	int adder_track = 1;

	for (i = 0; i < numbSelections; i++)
	{
		result.setTransition(0, adder_track, '^');
		med = selections.at(i);
		for (j = 0; j < med.lengthTransitions; j++)
		{
			newTransition = med.transitions[j];
			lengthTransitions++;
			result.setTransition(newTransition.vertexFrom + adder_track, newTransition.vertexTo + adder_track, newTransition.transitionSymbol);
		}
		adder_track += med.getVertexCount();

		result.setTransition(adder_track - 1, vertex_count - 1, '^');
	}

	result.setFinalState(vertex_count - 1);

	return result;
}

RegexParser RegexParser::iteration(RegexParser op1)
{
	RegexParser result;
	int i;
	transition newTransition;

	result.setVertex(op1.getVertexCount() + 2);

	result.setTransition(0, 1, '^');

	for (i = 0; i < op1.lengthTransitions; i++)
	{
		newTransition = op1.transitions[i];
		result.setTransition(newTransition.vertexFrom + 1, newTransition.vertexTo + 1, newTransition.transitionSymbol);
	}

	result.setTransition(op1.getVertexCount(), op1.getVertexCount() + 1, '^');
	result.setTransition(op1.getVertexCount(), 1, '^');
	result.setTransition(0, op1.getVertexCount() + 1, '^');

	result.setFinalState(op1.getVertexCount() + 1);

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

void RegexParser::setTransition(int vertexFrom, int vertexTo, char transitionSymbol)
{
	transition newTransition;
	newTransition.vertexFrom = vertexFrom;
	newTransition.vertexTo = vertexTo;
	newTransition.transitionSymbol = transitionSymbol;
	transitions[lengthTransitions] = newTransition;
	lengthTransitions++;
}

void RegexParser::setFinalState(int finState)
{
	finalState = finState;
}

int RegexParser::getVertexCount()
{
	return sizeVertex;
}

int RegexParser::getFinalState()
{
	return finalState;
}

//remainingWord, specialVertexes[currentVertex].transitionSymbol[i]
bool RegexParser::conditionCharEqual(string str, char ch)
{
	//remainingWord != "" && specialVertexes[currentVertex].transitionSymbol[i] == remainingWord[0]
	if (str != "" && ch == str[0]) return true;
	if (str != "" && (ch == '\a' && int(str[0]) >= 97 && int(str[0]) <= 122)) 
	{
		return true;
	}
	if (str != "" && (ch == '\d' && int(str[0]) >= 48 && int(str[0]) <= 57))
	{
		return true;
	}
	if (str != "" && (ch == '\s' && int(str[0]) == 32 || int(str[0]) == 9) || int(str[0]) == 10) 
	{
		return true;
	}
	if (str != "" && (ch == '\e' && int(str[0]) == 0))
	{
		return true;
	}

	return false;
}
