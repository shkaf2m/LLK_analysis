#include "grammar.hpp"
#include <string>
#include <vector>
#include <set>
#include <queue>

// Приведение грамматики
void GrammarCast(Grammar& g) {
  // 1) удаление бесплодных символов
  {
    std::set<Symbol> new_alphabet;
    std::set<Symbol> buffer_alphabet;
    
    for (auto& s : g.terminals) {
      new_alphabet.insert(s);
    }
    
    for (int _ = 0; _ < g.nonterminals.size(); ++_) {
      // пытаемся добавить новые нетерминальные символы
      for (Symbol& A : g.nonterminals) {
        if (new_alphabet.find(A) != new_alphabet.end()) {
          // символ уже лежит
          continue;
        }
        // необходимо существование подходящего правила
        for (Rule& rule : g.rules) {
          if (rule.left != A) {
            continue;
          }
          // rule.right in (new_alphabet)* (?)
          bool append_flag = true;
          for (Symbol& b : rule.right) {
            if (new_alphabet.find(b) == new_alphabet.end()) {
              append_flag = false;
              break;
            }
          }
          if (append_flag) {
            buffer_alphabet.insert(A);
            break;
          }
        }
      }
      if (buffer_alphabet.size() == 0) {
        // не смогли ничего добавить
        break;
      }
      for (Symbol nonterminal : buffer_alphabet) {
        new_alphabet.insert(nonterminal);
      }
      buffer_alphabet.clear();
    }

    // удаляем все нетерминалы, не попавшие в new_alphabet
    std::vector<Symbol> new_nonterminals;
    for (Symbol nonterminal : g.nonterminals) {
      if (new_alphabet.find(nonterminal) != new_alphabet.end()) {
        new_nonterminals.push_back(nonterminal);
      }
    }
    g.nonterminals = new_nonterminals;
  }


  // 2) удаление недостижимых
  {
    // пишем BFS для символов
    std::set<Symbol> new_alphabet;
    std::queue<Symbol> bfs_queue;
    bfs_queue.push(g.start_symbol);
    new_alphabet.insert(g.start_symbol);
    while (!bfs_queue.empty()) {
      Symbol top = bfs_queue.front();
      bfs_queue.pop();
      for (Rule& r : g.rules) {
        if (new_alphabet.find(r.left) == new_alphabet.end()) {
          continue;
        }
        // тогда можем добавить всю правую часть
        for (Symbol& s : r.right) {
          if (new_alphabet.find(s) == new_alphabet.end()) {
            new_alphabet.insert(s);
            bfs_queue.push(s);
          }
        }
      }
    }

    // оставляем только достижимые
    std::vector<Symbol> new_terminals;
    for (Symbol& s : g.terminals) {
      if (new_alphabet.find(s) != new_alphabet.end()) {
        new_terminals.push_back(s);
      }
    }

    std::vector<Symbol> new_nonterminals;
    for (Symbol& s : g.nonterminals) {
      if (new_alphabet.find(s) != new_alphabet.end()) {
        new_nonterminals.push_back(s);
      }
    }
    
    g.terminals = new_terminals;
    g.nonterminals = new_nonterminals;

    // дополнительно удалим правила, содержащие удаленные символы
    std::vector<Rule> new_rules;
    for (Rule& r : g.rules) {
      if (new_alphabet.find(r.left) == new_alphabet.end()) {
        continue;
      }
      for (Symbol& s : r.right) {
        if (new_alphabet.find(s) == new_alphabet.end()) {
          continue;
        }
      }
      new_rules.push_back(r);
    }
    
    g.rules = new_rules;
  }

  // 3) добавление S' -> S$
  {
    Symbol new_start_symbol = g.start_symbol + "'";
    Symbol marker_eof = "$";
    g.nonterminals.push_back(new_start_symbol);
    g.nonterminals.push_back(marker_eof);


    Rule new_rule = Rule{};
    new_rule.left = new_start_symbol;
    new_rule.right = {g.start_symbol, marker_eof};
    g.rules.push_back(new_rule);
    g.start_symbol = new_start_symbol;
  }
}