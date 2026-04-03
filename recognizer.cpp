#include "recognizer.hpp"
#include <list>
#include <algorithm>
#include <iostream>
#include <regex>


std::ostream& operator<<(std::ostream& output, std::list<Symbol> l) {
  // хитрость
  auto v = Word{l.begin(), l.end()};
  output << v;
  return output;
}


Recognizer::Recognizer(Grammar* g, FamilyTables* f) : grammar(g), family_tables(f){};

// на ленте только три вида символов
// 1) терминалы g.terminals
// 2) T_{i}, i от 0 до size(family_tables)
// 3) $


// вспомогательные функции

// получить по i => T_{i}
Symbol get_T(int i) {
  return "T_{" + std::to_string(i) + "}";
}

// получить по T_{i} => i
// -1, если символ не имеет вид T_{i}
int get_index_T(std::string s) {
  // std::regex pattern(R"(T_\d+)");
  std::regex pattern(R"(T_\{\d*\})");
  if (!std::regex_match(s, pattern)) {
    return -1;
  }

  std::string internal = "";
  for (int i = 3; i < s.size() - 1; ++i) {
    internal += s[i];
  }
  
  return std::stoi(internal);
}

int Recognizer::get_table_index(Symbol nonterm, std::set<Word> word_set) {
  // перебираем все таблицы и находим индекс
  for (int i = 0; i < family_tables->tables_vector.size(); ++i) {
    auto& key = family_tables->tables_vector[i].first;
    if (key.nonterm == nonterm && key.word_set == word_set) {
      return i;
    }
  }
  // не нашли
  return -1;
}

// вспомогательная функция для сложного шага
std::pair<Word, int> Recognizer::do_hard_step(Symbol special_symbol, Word u) {
  // вытаскиваем индекс
  int table_index = get_index_T(special_symbol);
  // std::cout << "index: " << table_index << '\n';


  // вытаскиваем объект таблицы
  auto& one_table = family_tables->tables_vector[table_index];

  // std::cout << one_table.first.nonterm << ' ' << one_table.first.word_set << '\n';

  // std::cout << "Пробую " << u << ' ' << family_tables->k << '\n';

  auto table_result = one_table.second.Get(u);
  // std::cout << table_result.status << '\n';
  if (table_result.status != 0) {
    // ошибочка, возвращаем инвариантное значение
    // std::cout << "Вернул; " << table_result.status << '\n';
    return std::make_pair(Word{}, -1);
  }

  // есть корректное правило
  Rule rule = table_result.rule;
  // std::cout << "Rule: " << rule << '\n';

  int rule_index = 0;
  for (int ind = 0; ind < grammar->rules.size(); ++ind) {
    if (grammar->rules[ind] == rule) {
      rule_index = ind;
      break;
    }
  }

  // начинаем строить ответ
  Word result_word;

  int nonterm_index = 0;


  for (Symbol s : rule.right) {
    if (grammar->IsTerminal(s)) {
      // std::cout << "Терминальный символ: " << s << '\n';
      result_word.push_back(s);
    } else if (s == epsilon) {
      continue;
    } else {
      // ищем нужную таблицу по (s, y[nonterm_index])
      // std::cout << "FIND " << s << ' ' << table_result.list_subsets[nonterm_index].word_set << '\n';
      int temp_table_index = get_table_index(s, table_result.list_subsets[nonterm_index].word_set);
      if (temp_table_index == -1) {
        // error
        // std::cout << "ERROR\n";
        return std::make_pair(Word{}, -1);
      }
      ++nonterm_index;
      result_word.push_back(get_T(temp_table_index));
    }
  }

  // std::cout << "Нужно положить " << result_word << '\n';

  return std::make_pair(result_word, rule_index);
}



ParsingResult Recognizer::Recognize(Word word) {

  ParsingResult res;

  // последовательность используемых правил
  std::vector<int> order_rules;
  std::string status;

  // заводим магазин
  std::list<Symbol> store_list;
  store_list.push_back("T_{0}");
  store_list.push_back("$");

  // переводим слово в список, для удобства
  std::list<Symbol> input_list;
  for (Symbol s : word) {
    input_list.push_back(s);
  }

  // ВАЖНООООООООООООООООООООООООООООООООООООООООООООООООООООООО
  input_list.push_back("$");

  // начинаем разбор
  while (true) {
    // std::cout << "--------\n";
    // std::cout << "Магазин: " << store_list << '\n';
    // std::cout << "Входная лента: " << input_list << '\n';
    // очевидный выход
    if (store_list.front() == "$" && input_list.size() == 0) {
      // accept!
      // std::cout << "ACCEPT!\n";
      res.status = "accept";
      res.rules = order_rules;
      return res;
    }

    // стандартный pop
    if (grammar->IsTerminal(store_list.front()) && store_list.front() == input_list.front()) {
      // pop!
      store_list.pop_front();
      input_list.pop_front();
      continue;
    }

    // проверка на ошибку
    // if (store_list.size() == 0) {
    //   std::cout << "Пусто\n";
    // } else {
    //   std::cout << "Трогаю " << store_list.front() << '\n';
    // }
    if (store_list.size() == 0 || get_index_T(store_list.front()) == -1) {
      // error
      // std::cout << "ERROR 1!\n";
      res.status = "error";
      return res;
    }

    // используем таблицу
    
    // первые k символ, или то, что осталось
    Word input_head;
    {
      int head_size = family_tables->k;

      // std::cout << head_size << '\n';

      if (head_size > input_list.size()) {
        head_size = input_list.size();
      }

      // std::cout << head_size << '\n';

      int cnt = 0;
      for (Symbol s : input_list) {
        cnt += 1;
        input_head.push_back(s);
        if (cnt >= head_size) {
          break;
        }
      }
    }

    Symbol special_symbol = store_list.front();
    
    // делегируем всю сложную работу
    auto pair = do_hard_step(special_symbol, input_head);
    if (pair.second == -1) {
      // error
      // std::cout << "ERROR 2!\n";
      res.status = "error";
      return res;
    }
    order_rules.push_back(pair.second);

    {
      // меняем спец символ на часть правила
      store_list.pop_front();
      for (int i = 0; i < pair.first.size(); ++i) {
        // не забываем перевернуть порядок
        store_list.push_front(pair.first[pair.first.size() - i - 1]);
      }
    }

  }
}