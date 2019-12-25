//
// Created by Gerardo Veltri 2019
//

#ifndef COMPILER_H
#define COMPILER_H

#include <list>
#include <string>

using namespace std;

class Node {
public:
 Node *left_ep;
 Node *right_ep;
 Node *next_node;
 char next_char;
 bool end;

 Node(char n, bool end);
 Node(char n, Node next_node, bool end);
 Node(bool end);
};

class NFA {
public:
 Node *start;
 Node *end;

 NFA(char n);
 NFA(NFA *nfa_left, NFA *nfa_right);
};


list<char> *postfixNotation(string source);
NFA *compileRegex(list<char> *pattern);
bool regexSearch(NFA *compiled_regex, list<char>& document);

#endif //CMAKE_COMPILER_H
