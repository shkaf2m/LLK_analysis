#include "first.hpp"
#include "grammar.hpp"
#include <map>
#include <set>


std::set<Word> FirstCalculator::RightSumK(std::set<Word>& lhs, std::set<Word>& rhs, int k, bool flag) {
  std::set<Word> result;
  for (Word x : lhs) {
    for (Word y : rhs) {
      Word z = x + y;

      if (z.size() <= k) {
        result.insert(z);
      } else if (flag) {
        for (Word temp_word : CalcFirst(z)) {
          result.insert(temp_word);
        }
      }
      
    }
  }
  return result;
}

FirstCalculator::FirstCalculator(Grammar g, int k) : grammar(g), k(k) {
  // вычислим базовые значения first для каждого символа из V_N объединить V_T

  // последовательные приближения
  std::map<Symbol, std::set<Word>> F_map;
  for (int i = 0;; ++i) {
    if (i == 0) {
      // init F_map
      for (Symbol term : g.terminals) {
        F_map[term] = std::set<Word>{Word{term}};
      }
      F_map[epsilon] = std::set<Word>{Word{epsilon}};

      for (Symbol nterm : g.nonterminals) {
        F_map[nterm] = std::set<Word>{};
        for (Rule rule : g.rules) {
          if (rule.left != nterm) {
            continue;
          }
          // смотрим на правую часть
          // она вся должна быть из терминалов
          if (rule.right.size() < k) {
            bool flag = true;
            for (Symbol s : rule.right) {
              if (!g.IsTerminal(s) && s != epsilon) {
                flag = false;
                break;
              }
            }
            if (flag) {
              F_map[nterm].insert(rule.right);
            }
          } else {
            // берем только первые k символов
            Word new_word{};
            bool flag = true;
            for (int j = 0; j < k; ++j) {
              new_word.push_back(rule.right[j]);
              if (!g.IsTerminal(rule.right[j]) && rule.right[j] != epsilon) {
                flag = false;
                break;
              }
            }
            if (flag) {
              F_map[nterm].insert(new_word);
            }
          }
        }
      }
      continue;
    }
    // i != 0
    // строим новый шаг
    std::map<Symbol, std::set<Word>> new_F_map;
    for (auto& s : g.terminals) {
      new_F_map[s] = F_map[s];
    }
    new_F_map[epsilon] = new_F_map[epsilon];
    for (auto& s : g.nonterminals) {
      new_F_map[s] = F_map[s];
    }
    for (Symbol& nterm: g.nonterminals) {
      for (Rule& rule : g.rules) {
        if (rule.left != nterm) {
          continue;
        }
        // строим сумму из F_(i-1)(Y_j) для каждого Y_j из правой части
        std::set<Word> result_set;
        for (int Y_ind = 0; Y_ind < rule.right.size(); ++Y_ind) {
          Symbol Y = rule.right[Y_ind];
          if (Y_ind == 0) {
            result_set = F_map[Y];
            continue;
          }
          std::set<Word> updated = RightSumK(result_set, F_map[Y], k, false);

          result_set = updated;
        }
        for (Word word : result_set) {
          new_F_map[nterm].insert(word);
        }

      }
    }
    // проверка на остановку
    bool flag_exit = true;
    for (Symbol& nterm : g.nonterminals) {
      if (new_F_map[nterm].size() != F_map[nterm].size()) {
        flag_exit = false;
        break;
      }
    }
    if (flag_exit) {
      // всё посчитали
      for (Symbol& term : g.terminals) {
        calculated_firsts[Word{term}] = new_F_map[term];
      }
      calculated_firsts[Word{epsilon}] = new_F_map[epsilon];
      for (Symbol& nterm : g.nonterminals) {
        calculated_firsts[Word{nterm}] = new_F_map[nterm];
      }
      return;
    } else {
      // меняем местами F_map и new_F_map и далее
      F_map = new_F_map;
    }
  }
}

std::set<Word> FirstCalculator::CalcFirst(Word w) {
  if (w.size() == 0) {
    return std::set<Word>{};
  }
  if (calculated_firsts.find(w) != calculated_firsts.end()) {
    return calculated_firsts[w];
  }
  // придется думать
  // посимвольная сумма по буквам слова
  std::set<Word> result;

  for (int S_ind = 0; S_ind < w.size(); ++S_ind) {
    Symbol s = w[S_ind];
    if (S_ind == 0) {
      // первый символ
      result = calculated_firsts[Word{s}];
      continue;
    }
    // прибавить к этому calculated_firsts[s]
    std::set<Word> new_result = RightSumK(result, calculated_firsts[Word{s}], k, true);
    result = new_result;
  }
  return result;
}