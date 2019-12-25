#include <iostream>
#include <fstream>
#include <list>
#include <queue>
#include <string>
#include <map>

#include <compiler.h>
#include <gvc.h>

static const char *const HEADER = "\nregex\n\n";
static const char *const USAGE = "Usage:\n\tregex <pattern> <file>\n\nDescription:\n\tSearches a document for a string.\n";

struct Edge {
   Node *src;
   Node *dst;
   char label;
 };

Agraph_t *NFAtoGraph(NFA input) {

 Agraph_t *g;
 Agedge_t *e;
 
 g = agopen("test-graph", Agdirected, 0);
 agattr(g, AGEDGE, "label", "");

 // Use Queue for BFS
 Node *start = input.start;
 queue<Node*> to_visit;
 to_visit.push(start);
 
 map<Node*, Agnode_t*> nodes;
 map<Node*, Agnode_t*>::iterator iter;
 Agnode_t *registered;
 Agnode_t *next;
 char state = 'A';
 char *node_name;
 Node *curr;
 while (!to_visit.empty()) {
   curr = to_visit.front();
   to_visit.pop();

   iter = nodes.find(curr);
   if (iter == nodes.end()) {
     node_name = (char*)malloc(sizeof(char)*1);
     node_name[0] = state++;
     registered = agnode(g, node_name, true);
     nodes[curr] = registered;
   }
   else {
     registered = nodes[curr];
   }

   if (curr->next_node != nullptr) {
     iter = nodes.find(curr->next_node);
     if (iter == nodes.end()) {
       node_name = (char*)malloc(sizeof(char)*1);
       node_name[0] = state++;
       next = agnode(g, node_name, 1);
       nodes[curr->next_node] = next;
     }
     else {
       next = nodes[curr->next_node];
     }
     e = agedge(g, registered, next, 0, 1);
     agset(e, "label", "char");

     to_visit.push(curr->next_node);
   }

   if (curr->left_ep != nullptr) {
     iter = nodes.find(curr->left_ep);
     if (iter == nodes.end()) {
       node_name = (char*)malloc(sizeof(char)*1);
       node_name[0] = state++;
       next = agnode(g, node_name, 1);
       nodes[curr->left_ep] = next;
     }
     else {
       next = nodes[curr->left_ep];
     }
     e = agedge(g, registered, next, 0, 1);
     agset(e, "label", "ep");

     to_visit.push(curr->left_ep);
   }

   if (curr->right_ep != nullptr) {
     iter = nodes.find(curr->right_ep);
     if (iter == nodes.end()) {
       node_name = (char*)malloc(sizeof(char)*1);
       node_name[0] = state++;
       next = agnode(g, node_name, 1);
       nodes[curr->right_ep] = next;
     }
     else {
       next = nodes[curr->right_ep];
     }
     e = agedge(g, registered, next, 0, 1);
     agset(e, "label", "ep");

     to_visit.push(curr->right_ep);
   }

 }

 return g;

}

void renderGraph(Agraph_t *g) {

 GVC_t *gvc;
 gvc = gvContext();

 gvLayout(gvc, g, "dot");

 gvRenderFilename(gvc, g, "png", "regex-NFA.png");

 gvFreeLayout(gvc, g);
 agclose(g);
 gvFreeContext(gvc);

 system("open ./regex-NFA.png");
}

int main(int argc, const char *argv[]) {


 std::cout << HEADER;

 // ensure the correct number of parameters are used.
 if (argc < 3) {
  std::cout << USAGE;
  return 1;
 }

 std::string regex_str(argv[1]);

 // read in document
 const char* file_name = argv[2];
 std::ifstream file;
 file.open(file_name);

 char c;
 std::list<char> document;
 while (file.get(c))
 {
  document.push_back(c);
 }

 std::cout << "PATTERN :: \n\n";

 for (char n : regex_str) {
  std::cout << n;
 }

 std::cout << "\n\nDOCUMENT :: \n\n";

 for (char n : document) {
  std::cout << n;
 }

 std::cout << "\n\nTEST POSTFIX :: \n\n";

 list<char> input = postfixNotation(regex_str);

 for (char n : input) {
  std::cout << n;
 }

 std::cout << "\n\n";

 std::cout << "\n\nTEST COMPILER :: \n\n";

 NFA compiled_pattern = compileRegex(input);

 Agraph_t *graph = NFAtoGraph(compiled_pattern);
 renderGraph(graph);

 return 0;
}
