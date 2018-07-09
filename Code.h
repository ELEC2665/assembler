#ifndef CODE_H
#define CODE_H

#include <string>
#include <map>

class Code
{
  public:
    Code();
    std::string dest(std::string mnemonic);
    std::string comp(std::string mnemonic);
    std::string jump(std::string mnemonic);

  private:
  // lookup tables for dest, comp and jump binary codes
    std::map<std::string, std::string> _dest_dict;
    std::map<std::string, std::string> _comp_dict;
    std::map<std::string, std::string> _jump_dict;
};

#endif