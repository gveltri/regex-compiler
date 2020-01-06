//
// Created by Gerardo Veltri 2019
//

#ifndef COMPILER_H
#define COMPILER_H

#include <list>
#include <string>

using namespace std;

class Edge;

class Node {
 public:
  list<Edge> edges;
  bool end;

  Node(char next_char, Node *next_node, bool end);
  Node(Node *left_ep, Node *right_ep);
  Node(bool end);
  void addEdge(char next_char, Node *next_node, bool cost);
};

class Edge {
public:
  Node *next;
  char cost;
  bool no_cost;
};


enum OperatorType { OP_CONCAT, OP_OR, OP_REPEAT, };

class NFA {
 public:
  Node *start;
  Node *end;
  list<Node*> nodes;

  NFA(char n);
  NFA(NFA nfa_left, NFA nfa_right, OperatorType op);
  void deleteContents();
};

list<char> postfixNotation(string source);
NFA compileRegex(list<char> pattern);
bool search(NFA compiled_regex, list<char> document);

#endif //COMPILER_H
