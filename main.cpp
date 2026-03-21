#include "pugixml.hpp"
#include <iostream>
#include <vector>
#include "parser.hpp"
#include "grammar.hpp"
#include "grammarcast.hpp"
#include "first.hpp"

void PrintWord(Word w) {
  std::cout << "{";
  for (auto& s : w) {
    std::cout << s << ", ";
  }
  std::cout << "}";
}

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

void PrintAllFirsts(Grammar& g, FirstCalculator& obj) {
  for (Symbol& term : g.terminals) {
    std::cout << "Term: " << term << '\n';
    for (auto& word : obj.calculated_firsts[Word{term}]) {
      std::cout << "Word: ";
      PrintWord(word);
      std::cout << '\n';
    }
  }
  for (Symbol& nterm : g.nonterminals) {
    std::cout << "NonTerm: " << nterm << '\n';
    for (auto& word : obj.calculated_firsts[Word{nterm}]) {
      std::cout << "Word: ";
      PrintWord(word);
      std::cout << '\n';
    }
  }
}

int main() {
  Grammar grammar = Parser::ParseFile("grammar2.xml");
  std::cout << "OLD GRAMMAR: \n";
  PrintGrammar(grammar);
  std::cout << "----------------------\n";
  GrammarCast(grammar);
  std::cout << "\nNEW GRAMMAR: \n";
  PrintGrammar(grammar);
  FirstCalculator obj{grammar, 1};
  PrintAllFirsts(grammar, obj);

  return 0;
}