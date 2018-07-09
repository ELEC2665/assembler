#include <iostream>
#include <fstream>
#include <string>
#include <bitset>
#include "Parser.h"
#include "Code.h"

int main(int argc, char const *argv[])
{
    std::string filename = argv[1];
    //std::cout << "Assembling " << filename << "...\n";
    
    Parser parser(filename);
    Code code;

    while ( parser.hasMoreCommands() ) {
        parser.advance();

        CommandType cmd_type = parser.commandType();

        if (cmd_type == CommandType::A) {
          std::string sym = parser.symbol();
          int val = std::stoi(sym,nullptr,10);
          std::string bin_val = std::bitset<16>(val).to_string();
          std::cout << bin_val << std::endl;

        }
        else if (cmd_type == CommandType::C) {
          std::string dest = parser.dest();
          std::string comp = parser.comp();
          std::string jump = parser.jump(); 
          //std::cout << "D:" << dest << " C:" << comp << " J:" << jump << std::endl;

          std::string dest_code = code.dest(dest);
          std::string comp_code = code.comp(comp);
          std::string jump_code = code.jump(jump);
          std::cout << "111" << comp_code << dest_code << jump_code << std::endl;
        }




    }

    //std::cout << "Done!\n";

    return 0;
}