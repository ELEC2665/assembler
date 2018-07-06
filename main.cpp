#include <iostream>
#include <fstream>
#include <string>
#include "Parser.h"

int main(int argc, char const *argv[])
{
    std::string filename = argv[1];
    std::cout << "Assembling " << filename << "...\n";
    
    Parser parser(filename);

    while ( parser.hasMoreCommands() ) {
        parser.advance();
        std::cout << "------< " << parser.jump() << std::endl;
    }

    std::cout << "Done!\n";

    return 0;
}