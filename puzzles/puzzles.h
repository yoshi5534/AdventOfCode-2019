#pragma once
#include <cstdint>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <computer.h>
#include <fuel.h>

using namespace AdventOfCode;

void solve1() {
  std::string const inputFilePath{"/workspaces/adventofcode2019/input/1.txt"};
  std::ifstream input{inputFilePath};
  if (!input.is_open())
    return;

  std::int64_t fuel = 0;
  std::string line{};
  while (std::getline(input, line))
    fuel += fuel_for_module (std::stoi (line));

  std::cout << "Required fuel: " << fuel << std::endl;
}

void solve2(){
  std::string const inputFilePath{"/workspaces/adventofcode2019/input/2.txt"};
  std::ifstream input{inputFilePath};
  if (!input.is_open())
    return;

  Program program {};
  std::string line{};
  while (std::getline(input, line, ','))
    program.push_back (std::stoi (line));

  {
    int noun = 12;
    int verb = 2;

    program [1] = noun;
    program [2] = verb;

    Computer computer (program);
    computer.calculate ();
    std::cout << "Result of puzzle 2a: " << computer.accessMemory()[0] << std::endl;
  }
  {
    int expected = 19690720;
    for (int noun = 0; noun < 100; ++noun) {
      for (int verb = 0; verb < 100; ++verb){
        program [1] = noun;
        program [2] = verb;

        Computer computer (program);
        computer.calculate ();
        auto result = computer.accessMemory()[0];
        if (result == expected)
        {
          int answer = 100 * noun + verb;
          std::cout << "Result of puzzle 2b: " << answer << std::endl;
          return;
        }
      }
    }
  }
}

void solve(int puzzleId, std::string const &inputFile) {
  if (puzzleId == 1)
    solve1();

  if (puzzleId == 2)
    solve2();
}