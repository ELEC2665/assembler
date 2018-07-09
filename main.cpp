#include <bitset>
#include <fstream>
#include <iostream>
#include <string>
#include "Code.h"
#include "Parser.h"

int main(int argc, char const *argv[]) {
  std::string filename = argv[1];
  std::cout << "Assembling " << filename << "...\n";

  // extract actual name (remove .asm)
  std::size_t pos = filename.find('.');
  std::string ofile_name = filename.substr(0, pos) + ".hack";
  // std::cout << "Output file = " << ofile_name << std::endl;
  std::ofstream ofile(ofile_name);

  Parser parser(filename);
  Code code;

  // keep looping while there are more commands in the file
  while (parser.hasMoreCommands()) {
    // go to the next command
    parser.advance();

    std::string current_cmd = parser.get_current_cmd();
    // check whether it is a valid command (i.e. not empty)
    if (!current_cmd.empty()) {
      std::cout << "###### Current Command = " << current_cmd << std::endl;

      // check the type of command and process
      CommandType cmd_type = parser.commandType();
      if (cmd_type == CommandType::A) {
        // convert the A-value into binary and store in string
        std::string sym = parser.symbol();
        int val = std::stoi(sym, nullptr, 10);
        std::string bin_val = std::bitset<16>(val).to_string();
        std::cout << "A: " << bin_val << std::endl;
        // print to to hack file
        ofile << bin_val << std::endl;

      } else if (cmd_type == CommandType::C) {
        // get the dest, compy and jump bits of a C-instruction
        std::string dest = parser.dest();
        std::string comp = parser.comp();
        std::string jump = parser.jump();
        std::cout << "Dest:" << dest << " Comp:" << comp << " Jump:" << jump
                  << std::endl;
        // convert these to binary codes
        std::string dest_code = code.dest(dest);
        std::string comp_code = code.comp(comp);
        std::string jump_code = code.jump(jump);
        // std::cout << "111" << comp_code << dest_code << jump_code <<
        // std::endl;
        // print to .hack file
        ofile << "111" << comp_code << dest_code << jump_code << std::endl;
      }
    }
  }

  ofile.close();  // remember to close output file
  // std::cout << "Done!\n";

  return 0;
}