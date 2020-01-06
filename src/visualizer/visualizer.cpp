//
// Created by Gerardo Veltri 2019
//

#include <queue>
#include <string>
#include <map>
#include <iostream>

#include "compiler.h"
#include <gvc.h>

struct GraphNode {
  Agnode_t* node_addr;
  bool visited;
};

Agraph_t *NFAtoGraph(NFA input) {

  Agraph_t *g = agopen("test-graph", Agdirected, 0);
  agattr(g, AGEDGE, "label", "");

  map<Node*, GraphNode> nodes;
  char state = 'a';
  char *node_name;

  for (Node *curr : input.nodes) {
    node_name = (char*)malloc(sizeof(char)*1);
    node_name[0] = state++;
    nodes[curr] = {agnode(g, node_name, true),
		    false};
  }

 

  Node *start = input.start;
  queue<Node*> to_visit;
  to_visit.push(start);
  nodes[start].visited = true;
 
  Node *curr;
  char *edge_name;
  while (!to_visit.empty()) {
    curr = to_visit.front();
    to_visit.pop();

    for (Edge e : curr->edges) {
      Agedge_t *ge = agedge(g,
			    nodes[curr].node_addr,
			    nodes[e.next].node_addr,
			    0, 1);

      if (!e.no_cost) {
	edge_name = (char*)malloc(sizeof(char)*1);
	edge_name[0] = e.cost;
	agset(ge, "label", edge_name);
      }

      if (!nodes[e.next].visited) {
	to_visit.push(e.next);
	nodes[e.next].visited = true;
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
