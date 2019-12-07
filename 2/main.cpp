#include <cstdint>
#include <iostream>
#include <string>
#include <sstream>

#include "computer.h"

int main (int argc, char** argv)
{
  using namespace AdventOfCode;
  int noun = 12;
  int verb = 2;

  if (argc == 3)
  {
    noun = std::stoi(argv[1]);
    verb = std::stoi(argv[2]);
  }

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

  program [1] = noun;
  program [2] = verb;

  Computer::calculate (program);
  std::cout << program [0];

  return 0;
}