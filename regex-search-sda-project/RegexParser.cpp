/**
*
* Solution to homework task
* Data Structures Course
* Faculty of Mathematics and Informatics of Sofia University
* Winter semester 2016/2017
*
* @author Stanislav Zmiycharov
* @idnumber 61883
* @task 0
* @compiler VC
*
*/

#include "RegexParser.h"

RegexParser::RegexParser()
{
	lengthTransitions = 0;
	countVertexes = 0;
}

//using Thompson's algorithm, iterate over string regex and construct NFA
RegexParser RegexParser::buildNFA(string regex)
{
	DynamicStack<char> operators;
	DynamicStack<RegexParser> operands;
	
	RegexParser *new_sym;
	char curSymbol;
	char operatorSymb;
	int operatorsCount;

	for (int i = 0; i < regex.size(); i++)
	{
		curSymbol = regex[i];
		if (curSymbol != '(' && curSymbol != ')' && curSymbol != '*' && curSymbol != '|' && curSymbol != '.') 
		{
			new_sym = new RegexParser();
			new_sym->increaseVertexCount(2);
			new_sym->setTransition(0, 1, curSymbol);
			new_sym->setFinalState(1);
			operands.Push(*new_sym);
			delete new_sym;
		}
		else
		{
			if (curSymbol == '*')
			{
				RegexParser operandSymb = operands.Top();
				operands.Pop();
				operands.Push(iteration(operandSymb));
			}
			else if (curSymbol == '.')
			{
				operators.Push(curSymbol);
			}
			else if (curSymbol == '|')
			{
				operators.Push(curSymbol);
			}
			else if (curSymbol == '(')
			{
				operators.Push(curSymbol);
			}
			// the symbol is ')'
			else
			{ 
				char c;

				operatorsCount = 0;
				operatorSymb = operators.Top();

				if (operatorSymb == '(') continue;

				do 
				{
					operators.Pop();
					operatorsCount++;
				} while (operators.Top() != '(');

				operators.Pop();
				
				RegexParser op1;
				RegexParser op2;
				RegexParser selections[50];
				
				if (operatorSymb == '.')
				{
					for (int i = 0; i < operatorsCount; i++)
					{
						op2 = operands.Top();
						operands.Pop();
						op1 = operands.Top();
						operands.Pop();
						operands.Push(concatenation(op1, op2));
					}
				}
				else if (operatorSymb == '|')
				{
					int trackerOperators = operatorsCount;

					for (int i = 0; i < operatorsCount + 1; i++)
					{
						selections[trackerOperators] = operands.Top();
						trackerOperators--;
						operands.Pop();
					}

					operands.Push(orSelection(selections, operatorsCount + 1));
				}
				else
				{
					std::cerr << "Regex parser encountered a problem while processing!\n";
					system("pause");
					exit(EXIT_FAILURE);
				}
			}
		}
	}

	// create the "special" vertexes, which represent our NFA as
	// index: value, where index == vertex number and the value shows
	// to which vertexes is the current one connected and with what symbol
	for (int i = 0; i < operands.Top().lengthTransitions; i++)
	{
		int vertSize = operands.Top().vertexes[operands.Top().transitions[i].vertexFrom].arrSize;
		int vertexFrom = operands.Top().transitions[i].vertexFrom;

		if (vertSize <= -1)
		{
			vertSize = 0;
		}

		if (vertexes[vertexFrom].arrSize <= -1)
		{
			operands.Top().vertexes[vertexFrom].arrSize = 0;
			vertexes[vertexFrom].arrSize = 0;
		}

		operands.Top().vertexes[vertexFrom].nextVert[vertSize] = operands.Top().transitions[i].vertexTo;
		operands.Top().vertexes[vertexFrom].transitionSymbol[vertSize] = operands.Top().transitions[i].transitionSymbol;
		operands.Top().vertexes[vertexFrom].arrSize++;

		vertexes[vertexFrom].nextVert[vertSize] = operands.Top().transitions[i].vertexTo;
		vertexes[vertexFrom].transitionSymbol[vertSize] = operands.Top().transitions[i].transitionSymbol;
		vertexes[vertexFrom].arrSize++;
	}

	while (vertexes[countVertexes].arrSize > 0)
	{
		countVertexes++;
	}

	operands.Top().countVertexes = countVertexes;

	return operands.Top();
}

bool RegexParser::match(int currentVertex, string remainingWord)
{
	//if we are at a final state match
	if (currentVertex == getFinalState()) return true;

	for (int i = 0; i < vertexes[currentVertex].arrSize; i++)
	{
		if (RegexParser::conditionEqual(remainingWord, vertexes[currentVertex].transitionSymbol[i]))
		{
			//if our letter matches - move forward the NFA and the string
			if (match(vertexes[currentVertex].nextVert[i], remainingWord.substr(1, string::npos))) return true;
		}
		else if (vertexes[currentVertex].transitionSymbol[i] == '^')
		{
			//if we have epsilon transition - move forward the NFA
			if (match(vertexes[currentVertex].nextVert[i], remainingWord)) return true;
		}
	}

	return false;
}

RegexParser RegexParser::concatenation(RegexParser op1, RegexParser op2)
{
	RegexParser result;
	result.increaseVertexCount(op1.getVertexCount() + op2.getVertexCount());
	transition newTransition;

	for (int i = 0; i < op1.lengthTransitions; i++)
	{
		newTransition = op1.transitions[i];
		result.setTransition(newTransition.vertexFrom, newTransition.vertexTo, newTransition.transitionSymbol);
	}

	result.setTransition(op1.getFinalState(), op1.getVertexCount(), '^');

	for (int i = 0; i < op2.lengthTransitions; i++)
	{
		newTransition = op2.transitions[i];
		result.setTransition(newTransition.vertexFrom + op1.getVertexCount(), newTransition.vertexTo + op1.getVertexCount(), newTransition.transitionSymbol);
	}

	result.setFinalState(op1.getVertexCount() + op2.getVertexCount() - 1);

	return result;
}

RegexParser RegexParser::orSelection(RegexParser selections[50], int numbSelections)
{
	RegexParser result;
	int vertexCount = 2;
	RegexParser curSelection;
	transition newTransition;

	for (int i = 0; i < numbSelections; i++) 
	{
		vertexCount += selections[i].getVertexCount();
	}

	result.increaseVertexCount(vertexCount);

	int adderCounter = 1;

	for (int i = 0; i < numbSelections; i++)
	{
		result.setTransition(0, adderCounter, '^');
		curSelection = selections[i];
		for (int j = 0; j < curSelection.lengthTransitions; j++)
		{
			newTransition = curSelection.transitions[j];
			lengthTransitions++;
			result.setTransition(newTransition.vertexFrom + adderCounter, newTransition.vertexTo + adderCounter, newTransition.transitionSymbol);
		}
		adderCounter += curSelection.getVertexCount();

		result.setTransition(adderCounter - 1, vertexCount - 1, '^');
	}

	result.setFinalState(vertexCount - 1);

	return result;
}

RegexParser RegexParser::iteration(RegexParser operandSymb)
{
	RegexParser result;
	transition newTransition;

	result.increaseVertexCount(operandSymb.getVertexCount() + 2);
	result.setTransition(0, 1, '^');

	for (int i = 0; i < operandSymb.lengthTransitions; i++)
	{
		newTransition = operandSymb.transitions[i];
		result.setTransition(newTransition.vertexFrom + 1, newTransition.vertexTo + 1, newTransition.transitionSymbol);
	}

	result.setTransition(operandSymb.getVertexCount(), operandSymb.getVertexCount() + 1, '^');
	result.setTransition(operandSymb.getVertexCount(), 1, '^');
	result.setTransition(0, operandSymb.getVertexCount() + 1, '^');

	result.setFinalState(operandSymb.getVertexCount() + 1);

	return result;
}

void RegexParser::increaseVertexCount(int vertexCount)
{
	for (int i = 0; i < vertexCount; i++)
	{
		countVertexes++;
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

int RegexParser::getFinalState()
{
	return finalState;
}

int RegexParser::getVertexCount()
{
	return countVertexes;
}

// as our regex parser accepts special characters
//that replace multiple other characters, we must consider them when matching
bool RegexParser::conditionEqual(string str, char ch)
{
	//\f = whitespace; \b = digit; \a = letter from English alphabet
	if (str != "" && ch == str[0]) return true;
	if (str != "" && (ch == '\a' && int(str[0]) >= 97 && int(str[0]) <= 122)) return true;
	if (str != "" && (ch == '\b' && int(str[0]) >= 48 && int(str[0]) <= 57)) return true;
	if (str != "" && (ch == '\f' && int(str[0]) == 32 || int(str[0]) == 9) || int(str[0]) == 10) return true;

	return false;
}
