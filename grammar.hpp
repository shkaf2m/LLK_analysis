#ifndef grammar__
#define grammar__

#include <string>
#include <vector>

const std::string epsilon = "epsilon";

using Symbol = std::string;

using Word = std::vector<Symbol>;

int WordSize(Word);

Word operator+(Word, Word);

struct Rule {
  int order;
  Symbol left;
  Word right;
  Rule() = default;
};



struct Grammar {
  std::vector<Symbol> terminals;
  std::vector<Symbol> nonterminals;
  Symbol start_symbol;
  std::vector<Rule> rules;
  Grammar() = default;
  bool IsTerminal(Symbol);
};

#endif