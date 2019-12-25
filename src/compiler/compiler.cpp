//
// Created by Gerardo Veltri 2019
//

#include "compiler.h"
#include <iostream>
#include <stack>
#include <stdexcept>

Node::Node(char n, bool end) {
  this->next_char = n;
  this->end = end;
}

Node::Node(char n, Node *next_node, bool end) {
  this->next_char = n;
  this->next_node = next_node;
  this->end = end;
}

Node::Node(bool end) {
  this->end = end;
}

Node::Node(Node *left, Node *right) {
  this->left_ep = left;
  this->right_ep = right;
}

NFA::NFA(char n) {
  Node *second = new Node(true);
  Node *first = new Node(n, second, false);

  start = first;
  end = second;
}

NFA::NFA(NFA nfa_left, NFA nfa_right) {

  // concatenate
  Node *left_join = nfa_left.end;
  Node *right_join = nfa_right.start;
  left_join->end = false;
  left_join->left_ep = right_join;

  start = nfa_left.start;
  end = nfa_right.end;
}

NFA::NFA(NFA nfa_left, NFA nfa_right, OperatorType op) {

  Node *left_join;
  Node *right_join;
  Node *left_end;
  Node *right_end;
  Node *left_start;
  Node *right_start;

  Node* new_start;
  Node* new_end;

  switch (op) {
  case OP_CONCAT:
    left_join = nfa_left.end;
    right_join = nfa_right.start;
    left_join->end = false;
    left_join->left_ep = right_join;

    start = nfa_left.start;
    end = nfa_right.end;
    break;
  case OP_OR:
    left_end = nfa_left.end;
    right_end = nfa_right.end;
    left_start = nfa_left.start;
    right_start = nfa_right.start;

    new_start = new Node(left_start, right_start);
    new_end = new Node(true);

    left_end->end = false;
    right_end->end = false;
    left_end->left_ep = new_end;
    right_end->left_ep = new_end;

    start = new_start;
    end = new_end;
    break;
  }
  
}

int precedence(char n) {
  switch (n) {
  case '*':
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
    if (i>0) {
      if ((!isOperator(source[i]) ||
	   (source[i] == '('))
	  && !isOperator(source[i-1])) {
	input.push_back('.');
	input.push_back(source[i]);
      }
      else if (((source[i-1] == ')') | (source[i-1] == '*'))
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
    output.push_back(operators.top());
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
      NFA combined {first, second, OP_CONCAT};
      nfa_stack.push(combined);
    }
    else if (n == '|') {
      NFA second = nfa_stack.top();
      nfa_stack.pop();
      NFA first = nfa_stack.top();
      nfa_stack.pop();
      NFA combined {first, second, OP_OR};
      nfa_stack.push(combined);
    }
    else {
      std::cout << "\nIGNORING " << n;
    }
  }

  NFA final = nfa_stack.top();
  nfa_stack.pop();

  return final;
}
