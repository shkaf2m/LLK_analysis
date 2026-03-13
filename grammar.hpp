#ifndef grammar__
#define grammar__

#include <string>
#include <vector>

struct Rule {
  int order;
  std::string left;
  std::vector<std::string> right;
  Rule() = default;
};

struct Grammar {
  std::vector<std::string> terminals;
  std::vector<std::string> nonterminals;
  std::string start_symbol;
  std::vector<Rule> rules;
  Grammar() = default;
};

#endif