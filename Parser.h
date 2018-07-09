// nand2tetris Assembler
// Dr Craig A. Evans
// (c) University of Leeds 
// July 2018

#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include "SymbolTable.h"

enum class CommandType { A, C, L };

class Parser {
 public:
  Parser(std::string filename);
  ~Parser();
  bool has_more_commands();
  void advance();
  CommandType command_type();
  std::string get_current_cmd();
  std::string symbol();
  std::string dest();
  std::string comp();
  std::string jump();
  void reset();
  void first_pass(SymbolTable &sym_table);

 private:
    std::string _current_command;
    std::ifstream _input_file;

};

#endif