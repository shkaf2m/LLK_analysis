#include "tables.hpp"
#include <cmath>

Y_Type::Y_Type(std::set<Word> s) : word_set(s) {};

std::vector<Y_Type> OneTable::getParsedRule(Rule r) {
  std::vector<Word> x_list;
  std::vector<Symbol> B_list;
  // разбиваем правую часть

  Word x_buffer;
  for (Symbol s : r.right) {
    if (grammar->IsTerminal(s) || s == epsilon) {
      x_buffer.push_back(s);
    } else {
      // собрали полноценный x
      // а может и нет, вдруг он пуст
      x_list.push_back(x_buffer);
      x_buffer.clear();
      // не забываем про нетерминал
      B_list.push_back(s);
    }
  }
  if (!x_buffer.empty()) {
    x_list.push_back(x_buffer);
    x_buffer.clear();
  }

  // собираем игрики
  std::vector<Y_Type> result_list;
  // ВАЖНО, ВОЗМОЖНО START I = 1
  for (int i = 0; i < B_list.size(); ++i) {
    Word y_help_word;
    for (int j = i + 1; j < x_list.size(); ++j) {
      y_help_word = y_help_word + x_list[j];
      if (j < B_list.size()) {
        y_help_word.push_back(B_list[j]);
      }
    }
    auto first = firstcalc->CalcFirst(y_help_word);
    auto sum = firstcalc->RightSumK(first, word_set, firstcalc->k, true);
    result_list.push_back(Y_Type{sum});
  }
  return result_list;
}


TableValue::TableValue(Rule r, std::vector<Y_Type> v, int s) : rule(r), list_subsets(v), status(s) {};

bool operator==(FamilyTablesKey lhs, FamilyTablesKey rhs) {
  return lhs.nonterm == rhs.nonterm && lhs.word_set == rhs.word_set;
}


OneTable::OneTable(Symbol nonterm, std::set<Word> word_set, Grammar* g, FirstCalculator* firstcalc) : nonterm(nonterm),
  word_set(word_set), grammar(g), firstcalc(firstcalc) {}


TableValue OneTable::Get(Word word) {
  // проверка в кеше
  if (table_map.find(word) != table_map.end()) {
    return table_map[word];
  }
  // проверка, что правило всего одно
  Rule work_rule;
  {
    int rules_cnt = 0;
    for (Rule r : grammar->rules) {
      if (r.left != nonterm) {
        continue;
      }
      auto first = firstcalc->CalcFirst(r.right);
      auto sum = firstcalc->RightSumK(first, word_set, firstcalc->k, true);
      if (sum.find(word) != sum.end()) {
        rules_cnt += 1;
        work_rule = r;
        if (rules_cnt > 1) {
          // undefined, return empty value
          TableValue result_tablevalue;
          result_tablevalue.status = 1;
          return result_tablevalue;
        }
      }
    }
    if (rules_cnt == 0) {
      // error, return empty value
      TableValue result_tablevalue;
      result_tablevalue.status = 2;
      return result_tablevalue;
    }
  }

  // прошли дальше => единственное правило, с которым нужно работать
  // осталось его распарсить и получить игрики
  std::vector<Y_Type> y_array = getParsedRule(work_rule);

  // обновление кеша
  table_map[word] = TableValue{work_rule, y_array, 0};
  return table_map[word];
}

FamilyTablesKey::FamilyTablesKey(Symbol s, std::set<Word> word_set) : nonterm(s), word_set(word_set){};


FamilyTables::FamilyTables(Grammar* g, FirstCalculator* firstcalc, int k) : grammar(g), firstcalc(firstcalc), k(k) {
  // строим замыкание таблиц

  // первым идет OneTable{S, {epsilon}}
  OneTable start_table{g->start_symbol, std::set<Word>{Word{epsilon}}, g, firstcalc};
  tables_vector.emplace_back(FamilyTablesKey{g->start_symbol, std::set<Word>{Word{epsilon}}}, start_table);

  // далее строим замыкание

  while (true) {
    bool exit_flag = true;
    // перебор таблиц
    for (auto pair : tables_vector) {
      // перебор слов из терминалов длины k
      // наглый перебор
      size_t terminals_count = g->terminals.size();
      for (size_t big_index = 0; big_index < std::pow(terminals_count, k); ++big_index) {
        Word curr_word;
        size_t copy_big_index = big_index;
        for (size_t j = 0; j < k; ++j) {
          curr_word.push_back(grammar->terminals[copy_big_index % terminals_count]);
          copy_big_index /= terminals_count;
        }

        TableValue result = pair.second.Get(curr_word);
        if (result.status != 0) {
          continue;
        }

        // пытаемся добавить новое

        // вспомогательный массив - нетерминалы правой части правила
        std::vector<Symbol> nonterms;
        for (Symbol s : result.rule.right) {
          if (!g->IsTerminal(s) && s != epsilon) {
            nonterms.push_back(s);
          }
        }

        if (nonterms.size() != result.list_subsets.size()) {
          std::cout << "CRITICAL ERROR TABLE \n";
          exit(1);
        }
        for (int i = 0; i < result.list_subsets.size(); ++i) {
          FamilyTablesKey new_key{nonterms[i], result.list_subsets[i].word_set};
          
          // поиск такого ключа
          bool exist_key_flag = false;
          for (auto& exist_pair : tables_vector) {
            if (exist_pair.first == new_key) {
              exist_key_flag = true;
              break;
            }
          }
          if (!exist_key_flag) {
            exit_flag = false; // смогли добавить на этой итерации
            tables_vector.emplace_back(new_key, OneTable{nonterms[i], result.list_subsets[i].word_set, g, firstcalc});
          }
        }
      } 
    }
    if (exit_flag) {
      break;
    }
  }

  // std::cout << "Количество таблиц; " << tables_vector.size() << '\n';
  // for (auto pair : tables_vector) { 
  //   std::cout << pair.first.nonterm << ' ' << pair.first.word_set << '\n';
  // }
}