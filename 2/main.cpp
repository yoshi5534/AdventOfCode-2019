#include <cstdint>
#include <iostream>
#include <string>
#include <sstream>

#include "computer.h"

int main (int argc, char** argv)
{
  using namespace AdventOfCode;

  std::string program_text;
  getline (std::cin, program_text);
  std::stringstream program_stream ( program_text );

  Program program;
  int opcode;
  while (program_stream >> opcode)
  {
    program.push_back (opcode);
    program_stream.seekg (1, std::ios_base::cur);
  }

  program [1] = 12;
  program [2] = 2;
  
  Computer::calculate (program);
  std::cout << "Result: \n";
  for (auto const& opcode: program)
  {
    std::cout << opcode << ",";
  }
  std::cout << std::endl;

  return 0;
}