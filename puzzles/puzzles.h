#pragma once
#include <cstdint>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <fuel.h>

void solve1() {
  std::string const inputFilePath{"/workspaces/adventofcode2019/input/1.txt"};
  std::ifstream input{inputFilePath};
  if (!input.is_open())
    return;

  std::int64_t fuel = 0;
  std::string line{};
  while (std::getline(input, line))
    fuel += AdventOfCode::fuel_for_module (std::stoi (line));

  std::cout << "Required fuel: " << fuel << std::endl;
}

void solve(int puzzleId, std::string const &inputFile) {
  if (puzzleId == 1)
    solve1();
}