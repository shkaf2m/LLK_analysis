#ifndef __tables
#define __tables
#include "grammar.hpp"
#include "first.hpp"
#include <set>
#include <map>

struct Y_Type {
  std::set<Word> word_set;
  Y_Type(std::set<Word>);
};

struct TableValue {
  Rule rule;
  std::vector<Y_Type> list_subsets;
  // 0 - ok
  // 1 - undefined, существует два правила
  // 2 - error, ни одного правила нет
  int status;
  TableValue() = default;
  TableValue(Rule, std::vector<Y_Type>, int);
};

struct OneTable {
  Symbol nonterm; // A
  std::set<Word> word_set; // L
  std::map<Word, TableValue> table_map;
  Grammar* grammar;
  FirstCalculator* firstcalc;

  TableValue Get(Word);

  std::vector<Y_Type> getParsedRule(Rule);
  OneTable(Symbol, std::set<Word>, Grammar*, FirstCalculator*);
};

struct FamilyTablesKey {
  Symbol nonterm;
  std::set<Word> word_set;
  FamilyTablesKey(Symbol, std::set<Word>);
};

bool operator==(FamilyTablesKey, FamilyTablesKey);

struct FamilyTables {
  Grammar* grammar;
  FirstCalculator* firstcalc;

  // т.к. мы не можем сравнивать FamilyTablesKey между собой
  // то придется отказаться от map и использовать обычный массив пар
  std::vector<std::pair<FamilyTablesKey, OneTable>> tables_vector;
  int k;

  FamilyTables(Grammar*, FirstCalculator*, int);
};


#endif