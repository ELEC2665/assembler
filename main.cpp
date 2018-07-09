// nand2tetris Assembler
// Dr Craig A. Evans
// (c) University of Leeds 
// July 2018

#include <bitset>
#include <fstream>
#include <iostream>
#include <string>
#include "Code.h"
#include "Parser.h"
#include "SymbolTable.h"

int main(int argc, char const *argv[]) {
  std::string filename = argv[1];
  std::cout << "Assembling " << filename << "...\n";
  Parser parser(filename);
  Code code;
  SymbolTable sym_table;

  int rom_address = 0;
  // first-pass of code - read symbols
  while (parser.has_more_commands()) {
    // get the next command tyoe
    parser.advance();
    CommandType cmd_type = parser.command_type();
    // if an A or C instruction, it is a valid command so need to 
    // increments the ROM address for the instrictopn
    if (cmd_type == CommandType::A || cmd_type == (CommandType::C)) {
      rom_address++;
    } 
    // if it is a (LABEL), it is not a command, so address isn't incremented
    else if (cmd_type == CommandType::L) {
      std::string sym = parser.symbol();
      // extract the symbol, see if it already exists in the table
      if (sym_table.contains(sym) == false) {
        // if it doesn't, then add it in
        sym_table.add_entry(sym, rom_address);
      }
    }
  }

  // Now do a second-pass of the code, so go back to beginning
  parser.reset();
   // extract actual name (remove .asm)
  std::size_t pos = filename.find('.');
  std::string ofile_name = filename.substr(0, pos) + ".hack";
  // std::cout << "Output file = " << ofile_name << std::endl;
  std::ofstream ofile(ofile_name);

  // keep looping while there are more commands in the file
  while (parser.has_more_commands()) {
    // go to the next command
    parser.advance();
    std::string current_cmd = parser.get_current_cmd();
    // check whether it is a valid command (i.e. not empty)
    if (!current_cmd.empty()) {
      // check the type of command and process
      CommandType cmd_type = parser.command_type();

      if (cmd_type == CommandType::A) {
        // get the symbol of the A instruction
        std::string sym = parser.symbol();
        // check whether it is a number or a label
        bool has_only_digits =
            (sym.find_first_not_of("0123456789") == std::string::npos);
        
        int val = 0;
        if (has_only_digits) {  // sym is a number
          // convert the string to a decimal integer
          val = std::stoi(sym, nullptr, 10);
        } else {  // if it is a label
          // check if already in the table , add it if not (address starts at 16)
          if (sym_table.contains(sym) == false) {
            sym_table.add_entry(sym);
          }
          // now get the numeric address
          val = sym_table.get_address(sym);
        }

        // convert the decimal value into binary
        std::string bin_val = std::bitset<16>(val).to_string();
        // print to to hack file
        ofile << bin_val << std::endl;

      } else if (cmd_type == CommandType::C) {
        // get the dest, compy and jump bits of a C-instruction
        std::string dest_code = code.dest(parser.dest());
        std::string comp_code = code.comp(parser.comp());
        std::string jump_code = code.jump(parser.jump());
        // print to .hack file
        ofile << "111" << comp_code << dest_code << jump_code << std::endl;
      }
    }
  }

  //sym_table.print();
  ofile.close();  // remember to close output file
  std::cout << "Done!\n";

  return 0;
}