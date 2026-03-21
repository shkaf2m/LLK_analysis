#ifndef grammar__
#define grammar__

#include <string>
#include <vector>

using Symbol = std::string;

using Word = std::vector<Symbol>;

struct Rule {
  int order;
  Symbol left;
  Word right;
  Rule() = default;
};

const std::string epsilon = "epsilon";

struct Grammar {
  std::vector<Symbol> terminals;
  std::vector<Symbol> nonterminals;
  Symbol start_symbol;
  std::vector<Rule> rules;
  Grammar() = default;
};

#endif