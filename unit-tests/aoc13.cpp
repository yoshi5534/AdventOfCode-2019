#include <catch2/catch.hpp>

#include <arcade.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using namespace AdventOfCode;

TEST_CASE("Simple game") {
  Program game{104, 1, 104, 2, 104, 2, 104, 6, 104, 5, 104, 4, 99};
  ArcadeCabinet cabinet{game};
  int blocks = cabinet.drawTiles();

  REQUIRE(1 == blocks);
}

Program getProgram() {
  std::string program_text;
  std::ifstream input("../../13/input.txt");
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

  return program;
}

TEST_CASE("AOC_13") {
  // Program game = getProgram();
  // ArcadeCabinet cabinet{game};
  // int blocks = cabinet.drawTiles();

  // int expected = 320;
  // REQUIRE(expected == blocks);
}