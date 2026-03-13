#include "pugixml.hpp"
#include <iostream>
#include <vector>
#include "parser.hpp"
#include "grammar.hpp"


void PrintGrammar(Grammar& g) {
  std::cout << "start symbol: " << g.start_symbol << '\n';
  for (auto i : g.nonterminals) {
    std::cout << "nt: " << i << '\n';
  }
  for (auto i : g.terminals) {
    std::cout << "t: " << i << '\n';
  }
  for (auto i : g.rules) {
    std::cout << "rule: " << i.left << " -> ";
    for (auto j : i.right) {
      std::cout << j << ' ';
    }
    std::cout << '\n';
  }
}

int main() {
  Grammar grammar = Parser::ParseFile("grammar1.xml");
  PrintGrammar(grammar);
  return 0;
}