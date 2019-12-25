#include <iostream>
#include <fstream>
#include <list>
#include <string>

#include <compiler.h>
#include <gvc.h>

static const char *const HEADER = "\nregex\n\n";
static const char *const USAGE = "Usage:\n\tregex <pattern> <file>\n\nDescription:\n\tSearches a document for a string.\n";


Agraph_t *NFAtoGraph(NFA *input) {

 Agraph_t *g;
 Agnode_t *n, *m;
 Agedge_t *e;


 g = agopen("test-graph", Agdirected, 0);
 n = agnode(g, "a", 1);
 m = agnode(g, "b", 1);
 e = agedge(g, n, m, 0, 1);
 e = agedge(g, n, m, 0, 1);
 e = agedge(g, n, m, 0, 1);

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

 list<char> *input = postfixNotation(regex_str);

 for (char n : *input) {
  std::cout << n;
 }

 std::cout << "\n\n";

 std::cout << "\n\nTEST COMPILER :: \n\n";

 NFA *compiled_pattern = compileRegex(input);

// renderGraph();

 return 0;
}
