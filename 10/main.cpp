#include <asteroids.h>
#include <cstdint>
#include <iostream>
#include <sstream>
#include <string>

int main(int argc, char **argv) {
  using namespace AdventOfCode;

  AsteroidMap map;
  int height = 0;
  int width = 0;
  std::string line;
  while (std::getline(std::cin, line)) {
    width = 0;
    for (char c : line) {
      if (c == '#')
        map.insert({width, height});
      width++;
    }
    height++;
  }

  Asteroids asteroids{map, width, height};
  std::cout << "Maximum visible asteroids: " << asteroids.mostVisible()
            << std::endl;

  return 0;
}