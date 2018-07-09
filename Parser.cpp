#include "Parser.h"

Parser::Parser(std::string filename) {
  _input_file.open(filename);
  if (_input_file.is_open() == false) {
    std::cerr << "Error! " << filename << " not opened!" << std::endl;
    exit(EXIT_FAILURE);
  }
}
Parser::~Parser() { _input_file.close(); }

// check if it at the end of the file
bool Parser::has_more_commands() {
  char c = _input_file.peek();
  if (c == EOF) {
    return false;
  } else {
    return true;
  }
}

// go to the next command in the file
void Parser::advance() {
  std::string next_line;
  // keep looking for command until at the end of the file
  while (has_more_commands()) {
    // read next line in file
    std::getline(_input_file, next_line);
    // remove whitespace from next line (space and newlines)
    next_line.erase(std::remove(next_line.begin(), next_line.end(), ' '),
                    next_line.end());
    next_line.erase(std::remove(next_line.begin(), next_line.end(), '\n'),
                    next_line.end());
    next_line.erase(std::remove(next_line.begin(), next_line.end(), '\r'),
                    next_line.end());

    // check whether the line contains a comment
    std::size_t pos = next_line.find("//");
    if (pos != std::string::npos) {  // if it does,
      // create substring from beginning of line to //
      next_line = next_line.substr(0, pos);
    }
    // now check the length of the substring to see if a command exists
    if (next_line.length() > 0) {
      // if a valid command exists, update the current command and break
      _current_command = next_line;
      break;
    } else {
      // if not a valid command, clear the current command to prevent previous
      // value being used
      _current_command.clear();
    }
  }
}

CommandType Parser::command_type() {
  // look for A-instruction and label
  std::size_t found_a = _current_command.find("@");
  std::size_t found_l = _current_command.find("(");

  // if not, A or L, must be C
  if (found_a != std::string::npos) {
    return CommandType::A;
  } else if (found_l != std::string::npos) {
    return CommandType::L;
  } else {  // C is harder to find due to variable format
    return CommandType::C;
  }
}

std::string Parser::symbol() {
  std::string sym;
  // A-instruction
  if (_current_command[0] == '@') {
    // remove the @ symbol
    sym = _current_command.substr(1, _current_command.length() - 1);
  }

  // bracket positions ( and ) in string
  std::size_t pos1 = _current_command.find('(');
  std::size_t pos2 = _current_command.find(')');

  // (Label)
  if (pos1 != std::string::npos && pos2 != std::string::npos) {
    // extract the string inside the brackets ( and )
    sym = _current_command.substr(pos1 + 1, pos2 - pos1 - 1);
  }

  return sym;
}
std::string Parser::dest() {
  // 1. dest = comp ; jump
  // 2. dest = comp
  // 3. comp; jump
  // check for = which is needed for a destination
  std::size_t pos = _current_command.find('=');
  std::string sym;
  if (pos != std::string::npos) {
    // extract string to the left of =
    sym = _current_command.substr(0, pos);
  }

  // std::cout << "Dest = " << sym << std::endl;
  return sym;
}
std::string Parser::comp() {
  // 1. dest = comp ; jump
  // 2. dest = comp
  // 3. comp; jump
  // check for = or a ;
  std::size_t pos1 = _current_command.find('=');
  std::size_t pos2 = _current_command.find(';');
  std::string sym;
  // contains = and ;
  if (pos1 != std::string::npos && pos2 != std::string::npos) {
    // substring between = and ;
    sym = _current_command.substr(pos1 + 1, pos2 - pos1 - 1);
  }
  // contains only =
  else if (pos1 != std::string::npos) {
    // substring from = to end of string
    sym =
        _current_command.substr(pos1 + 1, _current_command.length() - pos1 - 1);
  }
  // contains only ;
  else if (pos2 != std::string::npos) {
    sym = _current_command.substr(0, pos2);
  }
  // std::cout << "Comp = " << sym << std::endl;
  return sym;
}

std::string Parser::jump() {
  // 1. dest = comp ; jump
  // 2. dest = comp
  // 3. comp; jump
  // check for ; to know if a jump command exists
  std::size_t pos1 = _current_command.find(';');
  std::string sym;
  // contains ;
  if (pos1 != std::string::npos) {
    // extract substring from ; to end of line
    sym =
        _current_command.substr(pos1 + 1, _current_command.length() - pos1 - 1);
  }
  // std::cout << "Jump = " << sym << std::endl;
  return sym;
}

void Parser::reset() {
  // go back to beginning of file stream
  _input_file.clear();
  _input_file.seekg(0);
}

void Parser::generate_symbol_table(SymbolTable &sym_table) {
  int rom_address = 0;
  // first-pass of code - read symbols
  while (has_more_commands()) {
    // get the next command tyoe
    advance();
    // if an A or C instruction, need to increments the ROM address
    if (is_a_or_c()) {
      rom_address++;
    }
    // if it is a (LABEL), it is not a command, so address isn't incremented
    else  {
      // extract the symbol...
      std::string sym = symbol();
      // see if it already exists in the table
      if (sym_table.contains(sym) == false) {
        // if it doesn't, then add it in
        sym_table.add_entry(sym, rom_address);
      }
    }
  }
}

std::string Parser::get_code(SymbolTable &sym_table) {
  std::string bin_string;
  // check whether it is a valid command (i.e. not empty)
  if (!_current_command.empty()) {
    // check the type of command and process
    CommandType cmd_type = command_type();

    if (cmd_type == CommandType::A) {
      // get the symbol of the A instruction
      std::string sym = symbol();
      // check whether it is a number or a label
      bool has_only_digits =
          (sym.find_first_not_of("0123456789") == std::string::npos);

      int val = 0;
      if (has_only_digits) {  // sym is a number
        // convert the string to a decimal integer
        val = std::stoi(sym, nullptr, 10);
      } else {  // sym is a label
        // check if already in the table , add it if not
        if (sym_table.contains(sym) == false) {
          sym_table.add_entry(sym);
        }
        // now get the numeric address
        val = sym_table.get_address(sym);
      }

      // convert the decimal value into binary
      bin_string = std::bitset<16>(val).to_string();
    } else if (cmd_type == CommandType::C) {
      // get the dest, compy and jump bits of a C-instruction
      Code code;
      std::string dest_code = code.dest(dest());
      std::string comp_code = code.comp(comp());
      std::string jump_code = code.jump(jump());
      bin_string = "111" + comp_code + dest_code + jump_code;
    }
  }
  return bin_string;
}

bool Parser::is_a_or_c() {
  // first check to see if the current command isn't
  // a blank line which can happen for comment only lines
  if (_current_command.length() == 0) {
    return false;
  }
  // then check whether it is an A or C instruction
  if (command_type() == CommandType::L) {
    return false;
  } else {
    return true;
  }
}
