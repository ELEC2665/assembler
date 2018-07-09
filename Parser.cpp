#include "Parser.h"

Parser::Parser(std::string filename) {
  // TODO: check for errors whiel opening file
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
  // keep looking for command until at the end of the file
  while (hasMoreCommands()) {
    // read next line in file
    std::getline(_input_file, next_line);
    // remove whitespace from next line (space and newlines)
    next_line.erase(std::remove(next_line.begin(), next_line.end(), ' '),
                    next_line.end());
    next_line.erase(std::remove(next_line.begin(), next_line.end(), '\n'),
                    next_line.end());
    next_line.erase(std::remove(next_line.begin(), next_line.end(), '\r'),
                    next_line.end());

    // check whether it contains a comment
    std::size_t pos = next_line.find("//");
    if (pos != std::string::npos) {
      // create substring from beginning of line to //
      next_line = next_line.substr(0, pos);
      std::cout << "Line = " << next_line << " Length = " << next_line.length() << std::endl;
    }
    // now check the length of the substring to see if a command exists
    if (next_line.length() > 0) {
      // if a valid command exists, update the current command and break
      _current_command = next_line;
      _n++;
      break;
    } else {
      // if not a valid command, clear the current command to prevent previous value being used
      _current_command.clear();
    }
    
  }  
  // std::cout << "[" << _n << "] " << _current_command << std::endl;
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
  // 1. dest = comp ; jump
  // 2. dest = comp
  // 3. comp; jump

  // check for = which is needed for a destination
  std::size_t pos = _current_command.find('=');

  std::string sym;
  // extract string to the left of =
  if (pos != std::string::npos) {
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

std::string Parser::get_current_cmd() { return _current_command; }
