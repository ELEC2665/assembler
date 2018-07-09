#include "Parser.h"

Parser::Parser(std::string filename) {
  _input_file.open(filename);
  if (_input_file.is_open() == false) {
    std::cerr << "Error! " << filename << " not opened!" << std::endl;
    exit(EXIT_FAILURE);
  }

}
Parser::~Parser() {
  _input_file.close();
}

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

// accessor method for current command
std::string Parser::get_current_cmd() { return _current_command; }

void Parser::reset() {
  _input_file.clear();
  _input_file.seekg(0);
}
