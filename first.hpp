#ifndef first__
#define first__
#include "grammar.hpp"
#include <map>
#include <set>




struct FirstCalculator {
  Grammar grammar;
  int k;
  std::map<Word, std::set<Word>> calculated_firsts;

  FirstCalculator(Grammar, int);
  std::set<Word> CalcFirst(Word);

  // bool = false - без использования FIRST
  std::set<Word> RightSumK(std::set<Word>&, std::set<Word>&, int, bool);
};


#endif