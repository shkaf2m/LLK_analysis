#include "grammar.hpp"

int WordSize(Word w) {
  int eps_count = 0;
  for (auto s : w) {
    if (s == epsilon) {
      eps_count++;
    }
  }
  return w.size() - eps_count;
}

Word operator+(Word lhs, Word rhs) {
  // skip epsilon
  Word result{};
  for (auto s : lhs) {
    if (s == epsilon) {
      continue;
    }
    result.push_back(s);
  }
  for (auto s : rhs) {
    if (s == epsilon) {
      continue;
    }
    result.push_back(s);
  }
  if (result.size() != 0) {
    return result;
  }
  return Word{epsilon};
}

bool Grammar::IsTerminal(Symbol value) {
  for (auto& s : terminals) {
    if (s == value) {
      return true;
    }
  }
  return false;
}