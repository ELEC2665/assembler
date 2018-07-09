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
  bool hasMoreCommands();
  void advance();
  CommandType commandType();
  std::string get_current_cmd();
  std::string symbol();
  std::string dest();
  std::string comp();
  std::string jump();

 private:
    std::string _current_command;
    std::ifstream _input_file;
    int _n;
};

#endif