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
  auto screen = cabinet.drawTiles();

  auto blocks =
      std::count_if(std::begin(screen), std::end(screen), [](auto const &tile) {
        return std::holds_alternative<Block>(tile.second);
      });

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
  Program program {2};
  int opcode;
  while (program_stream >> opcode) {
    program.push_back(opcode);
    program_stream.seekg(1, std::ios_base::cur);
  }

  return program;
}

void printScreen(int width, int height, ImageLayer image) {
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      auto tile = image[width * y + x];
      if (tile == 0)
        std::cout << ' ';
      if (tile == 1)
        std::cout << 'W';
      if (tile == 2)
        std::cout << '#';
      if (tile == 3)
        std::cout << '-';
      if (tile == 4)
        std::cout << '@';
      else
        std::cout << ' ';
    }
    std::cout << '\n';
  }
}

TEST_CASE("AOC_13") {
  Program game = getProgram();
  ArcadeCabinet cabinet{game};
  auto screen = cabinet.getScreen();

  printScreen(screen.width(), screen.height(), screen.singleLayer(0));
}