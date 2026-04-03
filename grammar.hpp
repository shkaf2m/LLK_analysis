#ifndef grammar__
#define grammar__

#include <string>
#include <vector>
#include <iostream>
#include <set>

const std::string epsilon = "epsilon";

using Symbol = std::string;


using Word = std::vector<Symbol>;

std::ostream& operator<<(std::ostream&, Word);
std::ostream& operator<<(std::ostream&, std::vector<Word>);
std::ostream& operator<<(std::ostream&, std::set<Word>);



int WordSize(Word);

Word operator+(Word, Word);

struct Rule {
  // int order;
  Symbol left;
  Word right;
  Rule() = default;
};

bool operator==(Rule, Rule);

std::ostream& operator<<(std::ostream&, Rule);


struct Grammar {
  std::vector<Symbol> terminals;
  std::vector<Symbol> nonterminals;
  Symbol start_symbol;
  std::vector<Rule> rules;
  Grammar() = default;
  bool IsTerminal(Symbol);
};

#endif