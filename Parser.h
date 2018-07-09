// nand2tetris Assembler
// Dr Craig A. Evans
// (c) University of Leeds
// July 2018

#ifndef PARSER_H
#define PARSER_H

#include <fstream>
#include <iostream>
#include <string>
#include "Code.h"
#include "SymbolTable.h"

enum class CommandType { A, C, L };

class Parser {
 public:
  Parser(std::string filename);
  ~Parser();
  bool has_more_commands();
  void advance();
  void reset();
  void generate_symbol_table(SymbolTable &sym_table);
  std::string get_code(SymbolTable &sym_table);
  bool is_a_or_c();

 private:
  std::string _current_command;
  std::ifstream _input_file;

  CommandType command_type();
  std::string symbol();
  std::string dest();
  std::string comp();
  std::string jump();
};

#endif