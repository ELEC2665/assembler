#include "Code.h"

Code::Code() {
  // these are all the binary codes for the commands
  _dest_dict[""] = "000";
  _dest_dict["M"] = "001";
  _dest_dict["D"] = "010";
  _dest_dict["MD"] = "011";
  _dest_dict["A"] = "100";
  _dest_dict["AM"] = "101";
  _dest_dict["AD"] = "110";
  _dest_dict["AMD"] = "111";

  _comp_dict["0"] = "0101010";
  _comp_dict["1"] = "0111111";
  _comp_dict["-1"] = "0111010";
  _comp_dict["D"] = "0001100";
  _comp_dict["A"] = "0110000";
  _comp_dict["!D"] = "0001101";
  _comp_dict["!A"] = "0110001";
  _comp_dict["-D"] = "0001111";
  _comp_dict["-A"] = "0110011";
  _comp_dict["D+1"] = "0011111";
  _comp_dict["A+1"] = "0110111";
  _comp_dict["D-1"] = "0001110";
  _comp_dict["A-1"] = "0110010";
  _comp_dict["D+A"] = "0000010";
  _comp_dict["D-A"] = "0010011";
  _comp_dict["A-D"] = "0000111";
  _comp_dict["D&A"] = "0000000";
  _comp_dict["D|A"] = "0010101";
  _comp_dict["M"] = "1110000";
  _comp_dict["!M"] = "1110001";
  _comp_dict["-M"] = "1110011";
  _comp_dict["M+1"] = "1110111";
  _comp_dict["M-1"] = "1110010";
  _comp_dict["D+M"] = "1000010";
  _comp_dict["D-M"] = "1010011";
  _comp_dict["M-D"] = "1000111";
  _comp_dict["D&M"] = "1000000";
  _comp_dict["D|M"] = "1010101";

  _jump_dict[""] = "000";
  _jump_dict["JGT"] = "001";
  _jump_dict["JEQ"] = "010";
  _jump_dict["JGE"] = "011";
  _jump_dict["JLT"] = "100";
  _jump_dict["JNE"] = "101";
  _jump_dict["JLE"] = "110";
  _jump_dict["JMP"] = "111";
}

// TODO: should really check whether the mnemonic exists firsts and return an error message if it doesn't
std::string Code::dest(std::string mnemonic) { return _dest_dict[mnemonic]; }
std::string Code::comp(std::string mnemonic) { return _comp_dict[mnemonic]; }
std::string Code::jump(std::string mnemonic) { return _jump_dict[mnemonic]; }