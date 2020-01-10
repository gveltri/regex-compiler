//
// Created by Gerardo Veltri 2019
//

#ifndef COMPILER_H
#define COMPILER_H

#include <list>
#include <string>
#include <memory>

class Edge;

class Node {
 public:
  std::list<Edge> edges;
  bool end;

  Node(char next_char, std::weak_ptr<Node> next_node, bool end);
  Node(std::weak_ptr<Node> left_ep, std::weak_ptr<Node> right_ep);
  Node(bool end);
  void addEdge(char next_char, std::weak_ptr<Node> next_node, bool cost);
};

class Edge {
public:
  std::weak_ptr<Node> next;
  char cost;
  bool no_cost;
};


enum OperatorType { OP_CONCAT, OP_OR, OP_REPEAT, };

class NFA {
 public:
  std::weak_ptr<Node> start;
  std::weak_ptr<Node> end;
  std::list<std::shared_ptr<Node>> nodes;

  NFA(char n);
  NFA(NFA nfa_left, NFA nfa_right, OperatorType op);
};

std::list<char> postfixNotation(std::string source);
NFA compileRegex(std::list<char> pattern);
bool search(NFA compiled_regex, std::list<char> document);

#endif //COMPILER_H
