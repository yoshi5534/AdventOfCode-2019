#include <catch2/catch.hpp>

#include <arcade.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using namespace AdventOfCode;

Program getProgram() {
  std::string program_text;
  std::ifstream input("/workspaces/adventofcode2019/13/input.txt");
  if (input.is_open()) {
    getline(input, program_text);
    input.close();
  }

  std::stringstream program_stream{program_text};
  Program program;
  int opcode;
  while (program_stream >> opcode) {
    program.push_back(opcode);
    program_stream.seekg(1, std::ios_base::cur);
  }

  program[0] = 2;
  return program;
}


TEST_CASE("AOC_13") {
  Program game = getProgram();
  ArcadeCabinet cabinet{game};
  cabinet.play ();
}