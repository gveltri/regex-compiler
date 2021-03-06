#include <iostream>
#include <fstream>
#include <list>
#include <string>
#include <map>

#include <compiler.h>
#include <visualizer.h>
#include <gvc.h>

static const char *const HEADER = "\nregex\n\n";
static const char *const USAGE = "Usage:\n\tregex <pattern> <file>\n\nDescription:\n\tSearches a document for a string.\n";

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
  while (file.get(c)) {
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

  std::list<char> input = postfixNotation(regex_str);

  for (char n : input) {
    std::cout << n;
  }

  std::cout << "\n\nTEST COMPILER :: \n\n";

  NFA compiled_pattern = compileRegex(input);

  std::cout << "success";

  std::cout << "\n\nCONVERT NFA TO GRAPH :: \n\n";

  Agraph_t *graph = NFAtoGraph(compiled_pattern);

  std::cout << "success";

  std::cout << "\n\nRENDER GRAPH :: \n\n";

  renderGraph(graph);

  std::cout << "success\n\n";

  std::cout << "\n\nCHECK REFERENCE COUNT :: \n\n";

  while (!compiled_pattern.nodes.empty()) {
    auto curr = compiled_pattern.nodes.front();
    compiled_pattern.nodes.pop_front();
    std::cout << curr << "\t" << curr.use_count() << "\n";
  }

  return 0;
}
