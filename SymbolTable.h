// nand2tetris Assembler
// Dr Craig A. Evans
// (c) University of Leeds 
// July 2018

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
  void print();
private:
  std::map<std::string,int> _sym_table;
  int _next_address;
};

#endif