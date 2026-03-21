#include "pugixml.hpp"
#include <iostream>
#include <vector>
#include "parser.hpp"
#include "grammar.hpp"
#include "grammarcast.hpp"


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
  std::cout << "OLD GRAMMAR: \n";
  PrintGrammar(grammar);
  GrammarCast(grammar);
  std::cout << "\nNEW GRAMMAR: \n";
  PrintGrammar(grammar);
  // std::cout << Tester::GrammarTest(grammar, 1) << '\n';
  return 0;
}