#include<iostream>
#include<vector>
#include<string>
#include<set>
#include<stack>
#include "RegexParser.h"


int main() 
{
	NFA required_nfa;
	required_nfa = required_nfa.re_to_nfa("(a.(b|c|d))");
	required_nfa.match("ad");

	system("pause");
	return 0;
}