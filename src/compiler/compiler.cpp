//
// Created by Gerardo Veltri 2019
//

#include "compiler.h"
#include <iostream>
#include <stack>
#include <stdexcept>

Node::Node(char n, bool end) {
 next_char = n;
 end = end;
}

Node::Node(char n, Node next_node, bool end) {
 next_char = n;

 end = end;
}

Node::Node(bool end) {
 end = end;
}

NFA::NFA(char n) {
 Node *second = new Node(true);
 Node *first = new Node(n, second, false);
 start = first;
 end = second;
}

NFA::NFA(NFA *nfa_left, NFA *nfa_right) {

 // concatenate
 Node *left_join = nfa_left->end;
 Node *right_join = nfa_right->start;
 left_join->end = false;
 left_join->left_ep = right_join;

 start = nfa_left->start;
 end = nfa_right->end;

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

list<char>* postfixNotation(std::string source) {

 list<char> *input = new list<char>;
 list<char> *output = new list<char>;
 stack<char> *operators = new stack<char>;

 // insert explicit concatenate operator
 for (int i=0;i<source.size();i++) {
  if (i==0) {
   input->push_back(source[i]);
  }
  if (i>0) {
   if ((!isOperator(source[i]) ||
       (source[i] == '('))
       && !isOperator(source[i-1])) {
     input->push_back('.');
     input->push_back(source[i]);
    }
   else if (((source[i-1] == ')') | (source[i-1] == '*'))
            && !isOperator(source[i])) {
     input->push_back('.');
     input->push_back(source[i]);
    }
   else {
    input->push_back(source[i]);
   }
  }
 }

 // shunting yard
 while (!input->empty()) {
  char curr = input->front();
  input->pop_front();

  if (!isOperator(curr)) {
   output->push_back(curr);
  }
  else if (curr == '(') {
   operators->push(curr);
  }
  else if (curr == ')') {
   if (operators->size() == 0)
    throw std::invalid_argument( "unmatched parentheses" );

   while (operators->top() != '(') {
    output->push_back(operators->top());
    operators->pop();
    if (operators->size() == 0)
     throw std::invalid_argument( "unmatched parentheses" );

   }
   operators->pop();

  }
  else {
   while (operators->size() > 0) {
    char top = operators->top();

    if (precedence(top) > precedence(curr)) {
     output->push_back(top);
     operators->pop();
    }
    else {
     break;
    }

   }
   operators->push(curr);

  }
 }

 while (!operators->empty()) {
  output->push_back(operators->top());
  operators->pop();
 }

 return output;

}


NFA *compileRegex(list<char> *pattern) {

 std::stack<NFA> nfa_stack;

 for (char n : *pattern) {

  // concatenate
  if (!isOperator(n)) {
   NFA *curr = new NFA(n);
   nfa_stack.push(*curr);

   if (nfa_stack.size() > 1) {
    NFA *second = &nfa_stack.top();
    nfa_stack.pop();
    NFA *first = &nfa_stack.top();
    nfa_stack.pop();
    NFA *combined = new NFA(first, second);
    nfa_stack.push(*combined);
   }
  }
  else {
   std::cout << "\nIGNORING " << n;
  }
 }

 NFA *final = &nfa_stack.top();
 nfa_stack.pop();

 return final;
}
