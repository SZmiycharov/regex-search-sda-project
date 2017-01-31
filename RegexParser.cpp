#include "RegexParser.h"

RegexParser::RegexParser()
{
	lengthTransitions = 0;
	lengthSpecialVertexes = 0;
}

RegexParser RegexParser::buildNFA(string regex)
{
	DynamicStack<char> operators;
	DynamicStack<RegexParser> operands;
	char operatorSymbol;
	int operatorCount;
	char currentSymbol;
	RegexParser *new_sym;

	for (int i = 0; i < regex.size(); i++)
	{
		currentSymbol = regex[i];
		if (currentSymbol != '(' && currentSymbol != ')' && currentSymbol != '*' && currentSymbol != '|' && currentSymbol != '.') {
			new_sym = new RegexParser();
			new_sym->setVertex(2);
			new_sym->setTransition(0, 1, currentSymbol);
			new_sym->setFinalState(1);
			operands.Push(*new_sym);
			delete new_sym;
		}
		else {
			if (currentSymbol == '*')
			{
				RegexParser curNFA = operands.Top();
				operands.Pop();
				operands.Push(iteration(curNFA));
			}
			else if (currentSymbol == '.')
			{
				operators.Push(currentSymbol);
			}
			else if (currentSymbol == '|')
			{
				operators.Push(currentSymbol);
			}
			else if (currentSymbol == '(')
			{
				operators.Push(currentSymbol);
			}
			else
			{
				operatorCount = 0;
				char c;
				operatorSymbol = operators.Top();
				if (operatorSymbol == '(') continue;
				do {
					operators.Pop();
					operatorCount++;
				} while (operators.Top() != '(');
				operators.Pop();
				RegexParser op1;
				RegexParser op2;
				RegexParser selections[10];
				if (operatorSymbol == '.')
				{
					for (int i = 0; i < operatorCount; i++)
					{
						op2 = operands.Top();
						operands.Pop();
						op1 = operands.Top();
						operands.Pop();
						operands.Push(concatenation(op1, op2));
					}
				}
				else if (operatorSymbol == '|')
				{
					for (int i = 0; i <= operatorCount + 1; ++i)
					{
						selections[i] = RegexParser();
					}
					int tracker = operatorCount;
					for (int i = 0; i < operatorCount + 1; i++)
					{
						selections[tracker] = operands.Top();
						tracker--;
						operands.Pop();
					}
					operands.Push(orSelection(selections, operatorCount + 1));
				}
				else
				{
					std::cerr << "Problem happened while parsing regex!";
					system("pause");
					exit(EXIT_FAILURE);
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

		cout << "\nHEREE: " << operands.Top().transitions[i].vertexFrom << " " 
			<< operands.Top().transitions[i].vertexTo 
			<< " " << operands.Top().transitions[i].transitionSymbol << " \n";

		
		operands.Top().specialVertexes[vertexFrom].nextVert[specVertSize] = operands.Top().transitions[i].vertexTo;
		operands.Top().specialVertexes[vertexFrom].transitionSymbol[specVertSize] = operands.Top().transitions[i].transitionSymbol;
		operands.Top().specialVertexes[vertexFrom].arrSize++;

		cout << specialVertexes[vertexFrom].nextVert[specVertSize] << " ";
		specialVertexes[vertexFrom].nextVert[specVertSize] = operands.Top().transitions[i].vertexTo;
		cout << specialVertexes[vertexFrom].nextVert[specVertSize] << " ";
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
		//cout << "q" << newTransition.vertexFrom << " --> q" << newTransition.vertexTo << " : Symbol - " << newTransition.transitionSymbol << endl;
	}
	//cout << "\nThe final state is q" << getFinalState() << endl;
}

RegexParser RegexParser::concatenation(RegexParser curNFA, RegexParser b)
{
	RegexParser result;
	result.setVertex(curNFA.getVertexCount() + b.getVertexCount());
	int i;
	transition newTransition;

	for (i = 0; i < curNFA.lengthTransitions; i++)
	{
		newTransition = curNFA.transitions[i];
		result.setTransition(newTransition.vertexFrom, newTransition.vertexTo, newTransition.transitionSymbol);
	}

	result.setTransition(curNFA.getFinalState(), curNFA.getVertexCount(), '^');

	for (i = 0; i < b.lengthTransitions; i++)
	{
		newTransition = b.transitions[i];
		result.setTransition(newTransition.vertexFrom + curNFA.getVertexCount(), newTransition.vertexTo + curNFA.getVertexCount(), newTransition.transitionSymbol);
	}

	result.setFinalState(curNFA.getVertexCount() + b.getVertexCount() - 1);

	return result;
}

RegexParser RegexParser::orSelection(RegexParser selections[10], int numbSelections)
{
	RegexParser result;
	int vertex_count = 2;
	int i, j;
	RegexParser med;
	transition newTransition;

	for (i = 0; i < numbSelections; i++) {
		vertex_count += selections[i].getVertexCount();
	}

	result.setVertex(vertex_count);

	int adder_track = 1;

	for (i = 0; i < numbSelections; i++)
	{
		result.setTransition(0, adder_track, '^');
		med = selections[i];
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

RegexParser RegexParser::iteration(RegexParser curNFA)
{
	RegexParser result;
	int i;
	transition newTransition;

	result.setVertex(curNFA.getVertexCount() + 2);

	result.setTransition(0, 1, '^');

	for (i = 0; i < curNFA.lengthTransitions; i++)
	{
		newTransition = curNFA.transitions[i];
		result.setTransition(newTransition.vertexFrom + 1, newTransition.vertexTo + 1, newTransition.transitionSymbol);
	}

	result.setTransition(curNFA.getVertexCount(), curNFA.getVertexCount() + 1, '^');
	result.setTransition(curNFA.getVertexCount(), 1, '^');
	result.setTransition(0, curNFA.getVertexCount() + 1, '^');

	result.setFinalState(curNFA.getVertexCount() + 1);

	return result;
}

void RegexParser::setVertex(int numVertex)
{
	for (int i = 0; i < numVertex; i++)
	{
		vert hlp;
		hlp.id = i;
		vertex.push_back(hlp);
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