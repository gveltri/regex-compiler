//
// Created by Gerardo Veltri 2019
//

#include <queue>
#include <string>
#include <map>

#include "compiler.h"
#include <gvc.h>

struct GraphNode {
  Agnode_t* node_addr;
  bool visited;
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
 
 map<Node*, GraphNode> nodes;
 map<Node*, GraphNode>::iterator iter;
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
     nodes[curr] = {registered, true};
   }
   else {
     registered = nodes[curr].node_addr;
   }

   if (curr->next_node != nullptr) {
     iter = nodes.find(curr->next_node);
     if (iter == nodes.end()) {
       node_name = (char*)malloc(sizeof(char)*1);
       node_name[0] = state++;
       next = agnode(g, node_name, 1);
       nodes[curr->next_node] = {next, false};
     }
     else {
       next = nodes[curr->next_node].node_addr;
     }
     e = agedge(g, registered, next, 0, 1);
     agset(e, "label", "char");

     if (!nodes[curr->next_node].visited) {
       to_visit.push(curr->next_node);
       nodes[curr->next_node].visited = true;
     }
   }

   if (curr->left_ep != nullptr) {
     iter = nodes.find(curr->left_ep);
     if (iter == nodes.end()) {
       node_name = (char*)malloc(sizeof(char)*1);
       node_name[0] = state++;
       next = agnode(g, node_name, 1);
       nodes[curr->left_ep] = {next, false};
     }
     else {
       next = nodes[curr->left_ep].node_addr;
     }
     e = agedge(g, registered, next, 0, 1);
     agset(e, "label", "ep");

     if (!nodes[curr->left_ep].visited) {
       to_visit.push(curr->left_ep);
       nodes[curr->left_ep].visited = true;
     }
   }

   if (curr->right_ep != nullptr) {
     iter = nodes.find(curr->right_ep);
     if (iter == nodes.end()) {
       node_name = (char*)malloc(sizeof(char)*1);
       node_name[0] = state++;
       next = agnode(g, node_name, 1);
       nodes[curr->right_ep] = {next, false};
     }
     else {
       next = nodes[curr->right_ep].node_addr;
     }
     e = agedge(g, registered, next, 0, 1);
     agset(e, "label", "ep");
     if (!nodes[curr->right_ep].visited) {
       to_visit.push(curr->right_ep);
       nodes[curr->right_ep].visited = true;
     }
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
