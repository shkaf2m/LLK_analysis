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


std::ostream& operator<<(std::ostream& output, Word w) {
  if (w.size() == 0 || w[0] == epsilon) {
    output << "epsilon";
  } else {
    for (auto s : w) {
      output << s;
    }
  }
  return output;
}

std::ostream& operator<<(std::ostream& output, std::vector<Word> word_array) {
  output << '{';
  for (int i = 0; i < word_array.size(); ++i) {

    output << word_array[i];

    if (i + 1 != word_array.size()) {
      output << ", ";
    }
  }
  output << '}';
  return output;
}

std::ostream& operator<<(std::ostream& output, std::set<Word> word_set) {
  int cnt = 0;
  output << '{';
  for (auto w : word_set) {
    output << w;
    ++cnt;
    if (cnt != word_set.size()) {
      output << ", ";
    }
  }
  output << '}';
  return output;
}

std::ostream& operator<<(std::ostream& output, Rule r) {
  output << '{' << r.left << " -> " << r.right << '}';
  return output;
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