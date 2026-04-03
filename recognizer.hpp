#ifndef recognizer__
#define recognizer__

#include "grammar.hpp"
#include "tables.hpp"
#include <set>

struct ParsingResult {
  // статус парсинга
  std::string status;

  // последовательность правил из grammar
  std::vector<int> rules;
};

struct Recognizer {
  Grammar* grammar;
  FamilyTables* family_tables;
  
  Recognizer(Grammar*, FamilyTables*);

  std::pair<Word, int> do_hard_step(Symbol, Word);
  
  int get_table_index(Symbol, std::set<Word>);

  ParsingResult Recognize(Word);
};

#endif