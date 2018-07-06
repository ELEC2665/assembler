#ifndef PARSER_H
#define PARSER_H

#include <string>

enum class CommandType
{
    A,
    C,
    L
};

class Parser
{
  public:
    Parser(std::string filename);
    bool hasMoreCommands();
    void advance();
    CommandType commandType();
    std::string symbol();
    std::string dest();
    std::string comp();
    std::string jump();
 private:
};

#endif