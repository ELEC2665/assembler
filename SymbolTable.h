#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <string>
#include <iostream>
#include <map>

class SymbolTable {
 public:
  SymbolTable();
  void add_entry(std::string symbol,int address);
  void add_entry(std::string symbol);
  bool contains(std::string symbol);
  int get_address(std::string symbol);
private:
  std::map<std::string,int> _sym_table;
  int _next_address;
};

#endif