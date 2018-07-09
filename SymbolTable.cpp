#include "SymbolTable.h"

SymbolTable::SymbolTable() {
  // pre-defined symbols
  _sym_table["R0"] = 0;
  _sym_table["R1"] = 1;
  _sym_table["R2"] = 2;
  _sym_table["R3"] = 3;
  _sym_table["R4"] = 4;
  _sym_table["R5"] = 5;
  _sym_table["R6"] = 6;
  _sym_table["R7"] = 7;
  _sym_table["R8"] = 8;
  _sym_table["R9"] = 9;
  _sym_table["R10"] = 10;
  _sym_table["R11"] = 11;
  _sym_table["R12"] = 12;
  _sym_table["R13"] = 13;
  _sym_table["R14"] = 14;
  _sym_table["R15"] = 15;
  _sym_table["SCREEN"] = 16384;
  _sym_table["KBD"] = 24576;
  _sym_table["SP"] = 0;
  _sym_table["LCL"] = 1;
  _sym_table["ARG"] = 2;
  _sym_table["THIS"] = 3;
  _sym_table["THAT"] = 4;
  // first address for variables in RAM is 16
  _next_address = 16;
}

// add an entry to the table, specifying the address
void SymbolTable::add_entry(std::string symbol, int address) {
  _sym_table.insert(std::pair<std::string, int>(symbol, address));
}

// overloaded method, adds entry and auto increments the RAM address
void SymbolTable::add_entry(std::string symbol) {
  _sym_table.insert(std::pair<std::string, int>(symbol, _next_address));
  _next_address++;
}

// check whether a symbol already exists or not
bool SymbolTable::contains(std::string symbol) {
  if (_sym_table.count(symbol)) {
    return true;
  } else {
    return false;
  }
}

// get the address for a particular symbol
int SymbolTable::get_address(std::string symbol) {
  std::map<std::string, int>::iterator it;
  it = _sym_table.find(symbol);
  int address = it->second;
  return address;
}

// print the table
void SymbolTable::print() {
  for (std::map<std::string,int>::iterator it = _sym_table.begin();
       it != _sym_table.end(); ++it)
    std::cout << it->first << " => " << it->second << std::endl;
}