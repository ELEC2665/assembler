#include "Parser.h"

Parser::Parser(std::string filename) {
  // TODO: check for errors
  _input_file.open(filename);
  _n = 0;
}

bool Parser::hasMoreCommands() {
  char c = _input_file.peek();
  if (c == EOF) {
    return false;
  } else {
    return true;
  }
}

void Parser::advance() {
  std::string next_line;
  _n++;

  // keep looking for command until at the end of the file
  while (hasMoreCommands()) {
    // read next line in file
    std::getline(_input_file, next_line);

    // check whether it is a comment
    std::size_t found = next_line.find("//");

    // if not a comment and not a blank line, must be a command
    if (found == std::string::npos && next_line.length() > 1) {
      break;
    }
  }
  // TODO: does not work properly when file ends in blank line or comment
  _current_command = next_line;
  std::cout << "[" << _n << "] " << _current_command << std::endl;
}

CommandType Parser::commandType() {
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
    sym = _current_command.substr(1, _current_command.length() - 1);
  }

  // bracket positions ( and ) in string
  std::size_t pos1 = _current_command.find('(');
  std::size_t pos2 = _current_command.find(')');

  // Label
  if (pos1 != std::string::npos && pos2 != std::string::npos) {
    sym = _current_command.substr(pos1 + 1, pos2 - pos1 - 1);
  }

  return sym;
}
std::string Parser::dest() {
  // check for = which is needed for a destination
  std::size_t pos = _current_command.find('=');

  std::string sym;
  // extract string to the left of =
  if (pos != std::string::npos) {
    sym = _current_command.substr(0, pos);
  }

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
    sym = _current_command.substr(pos1 + 1, pos2);
  }
  // contains only =
  else if (pos1 != std::string::npos) {
    sym =
        _current_command.substr(pos1 + 1, _current_command.length() - pos1 - 1);
  }
  // contains only ;
  else if (pos2 != std::string::npos) {
    sym = _current_command.substr(0, pos2);
  }

  return sym;
}

std::string Parser::jump() {
  // 1. dest = comp ; jump
  // 2. dest = comp
  // 3. comp; jump

  // check for = or a ;
  std::size_t pos1 = _current_command.find(';');

  std::string sym;

  // contains ;
  if (pos1 != std::string::npos) {
    sym = _current_command.substr(pos1 + 1, _current_command.length() - pos1 - 1);
  }

  return sym;
}