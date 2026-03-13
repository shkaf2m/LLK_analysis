#ifndef parser__
#define parser__
#include <string>
#include "grammar.hpp"

struct Parser {
  static Grammar ParseFile(std::string);
};

#endif