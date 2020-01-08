//
// Created by Gerardo Veltri 2019
//

#include "compiler.h"
#include <iostream>
#include <stack>
#include <stdexcept>

Node::Node(char next_char, std::weak_ptr<Node> next_node, bool end) :
  end(end) {

  Edge edge {next_node, next_char, false};
  edges.push_back(edge);
  
}

Node::Node(bool end) : end(end) {}

Node::Node(std::weak_ptr<Node> left_ep, std::weak_ptr<Node> right_ep) {

  Edge edge1 {left_ep, '0', true};
  edges.push_back(edge1);
  Edge edge2 {right_ep, '0', true};
  edges.push_back(edge2);

}

void Node::addEdge(char cost, std::weak_ptr<Node> next_node, bool no_cost) {

  Edge edge {next_node, cost, no_cost};
  edges.push_back(edge);
}

NFA::NFA(char n) {
  auto second = std::make_shared<Node>(true);
  auto first = std::make_shared<Node>(n, second, false);
  
  nodes.push_back(first);
  nodes.push_back(second);

  start = std::weak_ptr<Node>(first);
  end = std::weak_ptr<Node>(second);
}

NFA::NFA(NFA nfa_left, NFA nfa_right, OperatorType op) {

  if (op == OP_CONCAT) {
    auto left_end = nfa_left.end.lock();
    left_end->end = false;
    left_end->addEdge('0', nfa_right.start, true);

    start = nfa_left.start;
    end = nfa_right.end;
    nodes.splice(nodes.end(), nfa_left.nodes);
    nodes.splice(nodes.end(), nfa_right.nodes);
  }    
  else if (op == OP_OR) {
    auto new_start = std::make_shared<Node>(nfa_left.start, nfa_right.start);
    auto new_end = std::make_shared<Node>(true);

    auto left_end = nfa_left.end.lock();
    auto right_end = nfa_right.end.lock();
    left_end->end = false;
    right_end->end = false;
    left_end->addEdge('0', std::weak_ptr<Node>(new_end), true);
    right_end->addEdge('0', std::weak_ptr<Node>(new_end), true);

    start = std::weak_ptr<Node>(new_start);
    end = std::weak_ptr<Node>(new_end);
    nodes.splice(nodes.end(), nfa_left.nodes);
    nodes.splice(nodes.end(), nfa_right.nodes);
    nodes.push_front(new_start);
    nodes.push_back(new_end);

  }
  else if (op == OP_REPEAT) {
    auto left_end = nfa_left.end.lock();
    left_end->addEdge('0', nfa_left.start, true);

    start = nfa_left.start;
    end = nfa_left.end;
    nodes.splice(nodes.end(), nfa_left.nodes);
  }  
}

int precedence(char n) {
  switch (n) {
  case '+':
    return 3;
  case '.':
    return 2;
  case '|':
    return 1;
  case '(':
    return 0;
  case ')':
    return 0;
  default:
    return -1;
  }
}

bool isOperator(char n) {
  if (precedence(n) == -1)
    return false;
  else
    return true;
}

list<char> postfixNotation(std::string source) {

  list<char> input;
  list<char> output;
  stack<char> operators;

  // insert explicit concatenate operator
  for (int i=0;i<source.size();i++) {
    if (i==0) {
      input.push_back(source[i]);
    }
    else {
      if (false) {
	// TODO: implement consecutive '+' checking
      }
      else if ((!isOperator(source[i]) ||
	   (source[i] == '('))
	  && !isOperator(source[i-1])) {
	input.push_back('.');
	input.push_back(source[i]);
      }
      else if (((source[i-1] == ')') | (source[i-1] == '+'))
	       && !isOperator(source[i])) {
	input.push_back('.');
	input.push_back(source[i]);
      }
      else {
	input.push_back(source[i]);
      }
    }
  }

  // shunting yard
  while (!input.empty()) {
    char curr = input.front();
    input.pop_front();

    if (!isOperator(curr)) {
      output.push_back(curr);
    }
    else if (curr == '(') {
      operators.push(curr);
    }
    else if (curr == ')') {
      if (operators.size() == 0)
	throw std::invalid_argument( "unmatched parentheses" );

      while (operators.top() != '(') {
	output.push_back(operators.top());
	operators.pop();
	if (operators.size() == 0)
	  throw std::invalid_argument( "unmatched parentheses" );

      }
      operators.pop();

    }
    else {
      while (operators.size() > 0) {
	char top = operators.top();

	if (precedence(top) > precedence(curr)) {
	  output.push_back(top);
	  operators.pop();
	}
	else {
	  break;
	}

      }
      operators.push(curr);

    }
  }

  while (!operators.empty()) {
    char top = operators.top();
    if (top == '(') {
      throw std::invalid_argument("unmatched parentheses");
    }
    output.push_back(top);
    operators.pop();
  }

  return output;

}


NFA compileRegex(list<char> pattern) {

  std::stack<NFA> nfa_stack;

  for (char n : pattern) {
    // concatenate
    if (!isOperator(n)) {
      NFA curr {n};
      nfa_stack.push(curr);
    }
    else if (n == '.') {
      NFA second = nfa_stack.top();
      nfa_stack.pop();
      NFA first = nfa_stack.top();
      nfa_stack.pop();
      NFA newNFA {first, second, OP_CONCAT};
      nfa_stack.push(newNFA);
    }
    else if (n == '|') {
      NFA second = nfa_stack.top();
      nfa_stack.pop();
      NFA first = nfa_stack.top();
      nfa_stack.pop();
      NFA newNFA {first, second, OP_OR};
      nfa_stack.push(newNFA);
    }
    else if (n == '+') {
      NFA first = nfa_stack.top();
      nfa_stack.pop();
      NFA newNFA {first, first, OP_REPEAT};
      nfa_stack.push(newNFA);
    }
    else {
      std::cout << "\nIGNORING " << n;
    }
  }

  NFA final = nfa_stack.top();
  nfa_stack.pop();

  return final;
}
