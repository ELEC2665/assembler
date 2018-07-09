#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>

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

 private:
    std::string _current_command;
    std::ifstream _input_file;
};

#endif