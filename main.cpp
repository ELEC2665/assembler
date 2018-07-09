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

  // extract actual name (remove .asm)
  std::size_t pos = filename.find('.');
  std::string ofile_name = filename.substr(0, pos) + ".hack";
  // std::cout << "Output file = " << ofile_name << std::endl;
  std::ofstream ofile(ofile_name);

  Parser first_parser(filename);
  Code code;
  SymbolTable sym_table;

  int rom_address = 0;
  // first-pass of code - read symbols
  while (first_parser.has_more_commands()) {
    first_parser.advance();
    CommandType cmd_type = first_parser.command_type();
    if (cmd_type == CommandType::A || cmd_type == (CommandType::C)) {
      rom_address++;
    } else if (cmd_type == CommandType::L) {
      std::string sym = first_parser.symbol();

      if (sym_table.contains(sym) == false) {
        sym_table.add_entry(sym, rom_address);
      }
    }
  }

  Parser parser(filename);

  // keep looping while there are more commands in the file
  while (parser.has_more_commands()) {
    // go to the next command
    parser.advance();

    std::string current_cmd = parser.get_current_cmd();
    // check whether it is a valid command (i.e. not empty)
    if (!current_cmd.empty()) {
      // std::cout << "###### Current Command = " << current_cmd << std::endl;

      // check the type of command and process
      CommandType cmd_type = parser.command_type();
      if (cmd_type == CommandType::A) {
        // convert the A-value into binary and store in string
        std::string sym = parser.symbol();
        bool has_only_digits =
            (sym.find_first_not_of("0123456789") == std::string::npos);
        
        int val = 0;
        if (has_only_digits) {  // sym is a number
          // sym is a number
          val = std::stoi(sym, nullptr, 10);
        } else {  // must be a label
          // if label not already in the table , add it (address starts at 16)
          if (sym_table.contains(sym) == false) {
            sym_table.add_entry(sym);
          }
          // now get the address
          val = sym_table.get_address(sym);
        }

        std::string bin_val = std::bitset<16>(val).to_string();
        // std::cout << "A: " << bin_val << std::endl;
        // print to to hack file
        ofile << bin_val << std::endl;

      } else if (cmd_type == CommandType::C) {
        // get the dest, compy and jump bits of a C-instruction
        std::string dest_code = code.dest(parser.dest());
        std::string comp_code = code.comp(parser.comp());
        std::string jump_code = code.jump(parser.jump());
        // std::cout << "111" << comp_code << dest_code << jump_code <<
        // std::endl; print to .hack file
        ofile << "111" << comp_code << dest_code << jump_code << std::endl;
      }
    }
  }

  ofile.close();  // remember to close output file
  std::cout << "Done!\n";

  return 0;
}