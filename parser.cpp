#include "parser.hpp"
#include "pugixml.hpp"
#include <iostream>

Grammar Parser::ParseFile(std::string filename) {
  pugi::xml_document doc;
  pugi::xml_parse_result load_res = doc.load_file(filename.c_str());
  if (!load_res) {
    // incorrect file
    std::cout << "Parse Error: " << load_res.description() << '\n';
    return Grammar{};
  }

  pugi::xml_node grammar_node = doc.child("grammar");
  Grammar result_grammar;

  // nonterminals
  for (pugi::xml_node& node : grammar_node.children("NS")) {
    result_grammar.nonterminals.push_back(node.text().get());
  }

  // terminals
  for (pugi::xml_node& node : grammar_node.children("TS")) {
    result_grammar.terminals.push_back(node.text().get());
  }

  // start symbol
  result_grammar.start_symbol = grammar_node.child("SS").text().get();

  // rules
  for (pugi::xml_node& rule_node : grammar_node.children("PR")) {
    Rule new_rule;
    new_rule.left = rule_node.child("left").child("PRS").text().get();
    for (pugi::xml_node& right_node : rule_node.child("right").children("PRS")) {
      new_rule.right.push_back(right_node.text().get());
    }
    result_grammar.rules.push_back(new_rule);
  }
  return result_grammar;
}