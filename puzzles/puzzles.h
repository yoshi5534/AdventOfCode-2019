#pragma once
#include <cstdint>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <amplifier.h>
#include <computer.h>
#include <fuel.h>
#include <orbit.h>
#include <password.h>
#include <spaceimage.h>
#include <wires.h>

using namespace AdventOfCode;

void getInput (std::ifstream& input, int puzzleId){
  std::stringstream inputFilePath;
  inputFilePath << "/workspaces/adventofcode2019/input/" << puzzleId << ".txt";
  input.open (inputFilePath.str ());

  if (!input.is_open())
    throw new std::runtime_error ("Input file not found");
}

void solve1() {
  std::ifstream input;
  getInput (input, 1);

  std::int64_t fuel = 0;
  std::string line{};
  while (std::getline(input, line))
    fuel += fuel_for_module (std::stoi (line));

  std::cout << "Required fuel: " << fuel << std::endl;
}

void solve2(){
  std::ifstream input;
  getInput (input, 2);

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

void solve3 (){
  std::ifstream input;
  getInput (input, 3);

  std::string path_1 {};
  std::getline(input, path_1);

  std::string path_2 {};
  std::getline(input, path_2);

  WirePath wire_1 = fromString (path_1);
  WirePath wire_2 = fromString (path_2);
  Intersections locations = intersects(wire_1, wire_2);

  int shortestDistance = distances(locations)[0];
  std::cout << "Shortest distance to crossing: " << shortestDistance << std::endl;

  int steps = minimalSignalDelay(wire_1, wire_2, locations);
  std::cout << "Minimal signal delay steps: " << steps << std::endl;
}

void solve4 () {
  std::ifstream input;
  getInput (input, 4);

  std::string lower_limit {};
  std::getline(input, lower_limit, '-');

  std::string upper_limit {};
  std::getline(input, upper_limit, '-');

  int validPasswords = 0;
  for (int password = std::stoi(lower_limit); password <= std::stoi(upper_limit); ++password)
    if (isValidPassword(password))
      validPasswords++;

  std::cout << "Number of valid passwords: " << validPasswords << std::endl;
}

void solve5(){
  std::ifstream input;
  getInput (input, 5);

  Program program {};
  std::string line{};
  while (std::getline(input, line, ','))
    program.push_back (std::stoi (line));

  {
    Computer computer(program);
    computer.writeInput({1});
    computer.calculate();

    auto output = computer.readOutput();
    while (output == 0)
      output = computer.readOutput ();
    std::cout << "Result of puzzle 5a: " << output << std::endl;
  }
  {
    Computer computer(program);
    computer.writeInput({5});
    computer.calculate();

    auto output = computer.readOutput();
    std::cout << "Result of puzzle 5b: " << output << std::endl;
  }
}

void solve6(){ 
  std::ifstream input;
  getInput (input, 6);

  Map map;

  std::string entry;
  while (std::getline(input, entry)) 
    map.addOrbit(entry);

  std::cout << "Checksum: " << map.checksum() << std::endl;
  std::cout << "Minimum orbital transfer: "
            << map.minimumOrbitalTransfer("YOU", "SAN") << std::endl;
}

void solve7(){
  std::ifstream input;
  getInput (input, 7);

  Program program {};
  std::string line{};
  while (std::getline(input, line, ','))
    program.push_back (std::stoi (line));

  std::cout << "Maximum thruster signal: " << AmplifierChain::maxThrusterSignal(program) << std::endl;

  FeedbackLoop loop (program);
  std::cout << "Maximum output signal: " << loop.maxOutputSignal () << std::endl;
}

void solve8(){
  std::ifstream input;
  getInput (input, 8);

  std::stringstream content;
  content << input.rdbuf();
  auto image = SpaceImage::fromDigitalSendingNetwork(25, 6, content.str());
  std::cout << "Image checksum: " << image.checksum () << std::endl;

  image.print ();
}

void solve9(){
  std::ifstream input;
  getInput (input, 9);

  Program program {};
  std::string line{};
  while (std::getline(input, line, ','))
    program.push_back (std::stoi (line));

  {
    Computer computer(program);
    computer.writeInput ({1});
    computer.calculate();

    std::cout << "BOOST keycode: " << computer.readOutput () << std::endl;
  }
  {
    Computer computer(program);
    computer.writeInput ({2});
    computer.calculate();

    std::cout << "Coordinates of distress signal: " << computer.readOutput () << std::endl;
  }
}

void solve(int puzzleId, std::string const &inputFile) {
  if (puzzleId == 1)
    solve1();

  if (puzzleId == 2)
    solve2();

  if (puzzleId == 3)
    solve3();

  if (puzzleId == 4)
    solve4();

  if (puzzleId == 5)
    solve5();

  if (puzzleId == 6)
    solve6();

  if (puzzleId == 7)
    solve7();

  if (puzzleId == 8)
    solve8();

  if (puzzleId == 9)
    solve9();
}