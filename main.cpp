// nand2tetris Assembler
// Dr Craig A. Evans
// (c) University of Leeds
// July 2018

#include <bitset>
#include <fstream>
#include <iostream>
#include <string>
#include "Parser.h"
#include "SymbolTable.h"

std::string get_ofile_name(std::string filename);

int main(int argc, char const *argv[]) {
  std::string filename = argv[1];
  std::cout << "Assembling " << filename << "...\n";

  Parser parser(filename);
  SymbolTable sym_table;
  std::ofstream ofile(get_ofile_name(filename));

  // do a first-pass to generate symbol table
  parser.generate_symbol_table(sym_table);

  // Now do a second-pass of the code, so go back to beginning
  parser.reset();

  // keep looping while there are more commands in the file
  while (parser.has_more_commands()) {
    // go to the next command
    parser.advance();
    // if it is a A or C instruction i.e. not a label
    if (parser.is_a_or_c()) {
      // get binary code and print to output
      ofile << parser.get_code(sym_table) << std::endl;
    }
  }

  ofile.close();  // remember to close output file
  std::cout << "Done!\n";
  return 0;
}

std::string get_ofile_name(std::string filename) {
  // extract actual name (remove .asm)
  std::size_t pos = filename.find('.');
  return filename.substr(0, pos) + ".hack";
}