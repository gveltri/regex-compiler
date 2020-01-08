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

  Node(char next_char, weak_ptr<Node> next_node, bool end);
  Node(weak_ptr<Node> left_ep, weak_ptr<Node> right_ep);
  Node(bool end);
  void addEdge(char next_char, weak_ptr<Node> next_node, bool cost);
};

class Edge {
public:
  weak_ptr<Node> next;
  char cost;
  bool no_cost;
};


enum OperatorType { OP_CONCAT, OP_OR, OP_REPEAT, };

class NFA {
 public:
  weak_ptr<Node> start;
  weak_ptr<Node> end;
  list<shared_ptr<Node>> nodes;

  NFA(char n);
  NFA(NFA nfa_left, NFA nfa_right, OperatorType op);
};

list<char> postfixNotation(string source);
NFA compileRegex(list<char> pattern);
bool search(NFA compiled_regex, list<char> document);

#endif //COMPILER_H
