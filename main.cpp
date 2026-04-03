#include "pugixml.hpp"
#include <iostream>
#include <vector>
#include <cstring>
#include "parser.hpp"
#include "grammar.hpp"
#include "grammarcast.hpp"
#include "first.hpp"

#include "tables.hpp"
#include "recognizer.hpp"


void PrintGrammar(Grammar& g) {
  std::cout << "start symbol: " << g.start_symbol << '\n';
  for (auto i : g.nonterminals) {
    std::cout << "nt: " << i << '\n';
  }
  for (auto i : g.terminals) {
    std::cout << "t: " << i << '\n';
  }
  for (auto i : g.rules) {
    std::cout << "rule: " << i.left << " -> " << i.right << '\n';
  }
}

// void PrintAllFirsts(Grammar& g, FirstCalculator& obj) {
//   for (Symbol& term : g.terminals) {
//     std::cout << "Term: " << term << '\n';
//     std::cout << obj.calculated_firsts[Word{term}] << '\n';
//   }
//   for (Symbol& nterm : g.nonterminals) {
//     std::cout << "NonTerm: " << nterm << '\n';
//     std::cout << obj.calculated_firsts[Word{nterm}] << '\n';
//   }
// }

struct Args {
  std::string grammar_file;
  std::string word;
  int k = 1;
  bool valid = false;
};

Args parseArgs(int argc, char* argv[]) {
  Args args;
  

  for (int i = 1; i < argc; ++i) {
    if (strcmp(argv[i], "--grammar") == 0 && i + 1 < argc) {
      args.grammar_file = argv[i + 1];
    }

    if (strcmp(argv[i], "--word") == 0 && i + 1 < argc) {
      args.word = argv[i + 1];
    }
    
    if (strcmp(argv[i], "--k") == 0 && i + 1 < argc) {
      args.k = std::atoi(argv[i + 1]);
    }
  }


  if (args.grammar_file != "" && args.word != "") {
    args.valid = true;
  }
  return args;
}

int main(int argc, char* argv[]) {

  auto args = parseArgs(argc, argv);

  Grammar grammar = Parser::ParseFile(args.grammar_file);
  // std::cout << "OLD GRAMMAR: \n";
  // PrintGrammar(grammar);
  // std::cout << "----------------------\n";
  GrammarCast(grammar);
  std::cout << "\nNEW GRAMMAR: \n";
  PrintGrammar(grammar);
  FirstCalculator firstcalculat{grammar, args.k};
  // std::cout << "ALL firsts: \n";
  // PrintAllFirsts(grammar, firstcalculat);


  // // std::cout << "TEST\n";
  // // std::cout << firstcalculat.CalcFirst({"b", "A", "b", "a"}) << '\n';
  
  // OneTable my_table{grammar.start_symbol, std::set<Word>{Word{"epsilon"}}, &grammar, &firstcalculat};

  // std::cout << "TEST MY_TABLE\n";
  // TableValue bf;

  // bf = my_table.Get(Word{"a", "a"});
  // if (bf.status != 0) {
  //   std::cout << "ОШИБОЧКА ВЫШЛА " << bf.status << '\n';
  // } else {
  //   std::cout << bf.rule << '\n';
  //   std::cout << "Количество контекстов: " << bf.list_subsets.size() << '\n';
  //   for (auto y : bf.list_subsets) {
  //     std::cout << y.word_set << '\n';
  //   }
  // }

  // bf = my_table.Get(Word{"b", "a"});
  // if (bf.status != 0) {
  //   std::cout << "ОШИБОЧКА ВЫШЛА " << bf.status << '\n';
  // } else {
  //   std::cout << bf.rule << '\n';
  //   std::cout << "Количество контекстов: " << bf.list_subsets.size() << '\n';
  //   for (auto y : bf.list_subsets) {
  //     std::cout << y.word_set << '\n';
  //   }
  // }

  // bf = my_table.Get(Word{"a", "b"});
  // if (bf.status != 0) {
  //   std::cout << "ОШИБОЧКА ВЫШЛА " << bf.status << '\n';
  // } else {
  //   std::cout << bf.rule << '\n';
  //   std::cout << "Количество контекстов: " << bf.list_subsets.size() << '\n';
  //   for (auto y : bf.list_subsets) {
  //     std::cout << y.word_set << '\n';
  //   }
  // }

  // bf = my_table.Get(Word{"b", "b"});
  // if (bf.status != 0) {
  //   std::cout << "ОШИБОЧКА ВЫШЛА " << bf.status << '\n';
  // } else {
  //   std::cout << bf.rule << '\n';
  //   std::cout << "Количество контекстов: " << bf.list_subsets.size() << '\n';
  //   for (auto y : bf.list_subsets) {
  //     std::cout << y.word_set << '\n';
  //   }
  // }



  // OneTable start_table{"E", std::set<Word>{Word{"$"}}, &grammar, &firstcalculat};
  // std::cout << firstcalculat.CalcFirst(Word{"T", "E'"}) << '\n';
  // // Проверка верности первой таблицы
  // std::cout << "CHECKING\n";
  // for (auto s : grammar.terminals) {
  //   auto curr_res = start_table.Get(Word{s});
  //   std::cout << "Symbol: " << s << " status " << curr_res.status << '\n';
  //   if (curr_res.status == 0) {
  //     std::cout << curr_res.rule << '\n';
  //     for (auto tempo : curr_res.list_subsets) {
  //       std::cout << tempo.word_set << '\n';
  //     }
  //   }
  // }





  // // std::cout << firstcalculat.CalcFirst(Word{epsilon}) << '\n';
  // std::cout << firstcalculat.calculated_firsts[Word{epsilon}] << '\n';
  // OneTable my_table2{"A", std::set<Word>{Word{"b", "a"}}, &grammar, &firstcalculat};
  // std::cout << "TEST MY_TABLE 222222\n";
  // TableValue bf;

  // bf = my_table2.Get(Word{"b", "b"});
  // if (bf.status != 0) {
  //   std::cout << "ОШИБОЧКА ВЫШЛА\n";
  // } else {
  //   std::cout << bf.rule << '\n';
  //   for (auto y : bf.list_subsets) {
  //     std::cout << y.word_set << '\n';
  //   }
  // }

  // bf = my_table2.Get(Word{"b", "a"});
  // if (bf.status != 0) {
  //   std::cout << "ОШИБОЧКА ВЫШЛА " << bf.status << '\n';
  // } else {
  //   std::cout << bf.rule << '\n';
  //   // std::cout << "Количество контекстов: " << bf.list_subsets.size() << '\n';
  //   for (auto y : bf.list_subsets) {
  //     std::cout << y.word_set << '\n';
  //   }
  // }

  // bf = my_table2.Get(Word{epsilon});
  // if (bf.status != 0) {
  //   std::cout << "ОШИБОЧКА ВЫШЛА\n";
  // } else {
  //   std::cout << bf.rule << '\n';
  //   for (auto y : bf.list_subsets) {
  //     std::cout << y.word_set << '\n';
  //   }
  // }

  FamilyTables family_tables{&grammar, &firstcalculat, args.k};

  std::cout << "BUILDING TABLES: SUCCESS\n";

  Recognizer recogn{&grammar, &family_tables};

  // std::cout << "Start recognizer\n";
  // recogn.Recognize(Word{"a"});
  // recogn.Recognize(Word{"a", "+", "a"});
  // recogn.Recognize(Word{"a", "*", "a"});

  // recogn.Recognize(Word{"a", "+", "a", "*", "a"});
  // recogn.Recognize(Word{"(", "(", "a", ")", ")"});

  // recogn.Recognize(Word{"+", "a"}); // false
  // recogn.Recognize(Word{"a", "+"}); // false
  // recogn.Recognize(Word{"epsilon"}); // false


  // auto res = recogn.Recognize(Word{"b", "b", "a"});

  // парсинг слова
  std::vector<Symbol> test_word = Parser::ParseWord(args.word);
  // std::cout << "Слово: " << test_word << '\n';
  // auto res = recogn.Recognize(Word{"b", "b", "b", "a"});
  auto res = recogn.Recognize(test_word);
  if (res.status == "accept") {
    std::cout << "ACCEPT\n";
    std::cout << "Used rules:\n";
    for (int ind = 0; ind < res.rules.size(); ++ind) {
      std::cout << res.rules[ind] << ' ' << grammar.rules[res.rules[ind]] << '\n';
    }
  } else {
    std::cout << "REJECT\n";
  }
  // recogn.Recognize(Word{"a", "a", "a"});
  // recogn.Recognize(Word{"b", "b", "b", "a"});
  // recogn.Recognize(Word{"b", "b", "a", "a"}); //false

  return 0;
}