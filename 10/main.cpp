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
  auto station = asteroids.mostVisible();
  std::cout << "Maximum visible asteroids: " << station.second << std::endl;
  std::cout << "Coordinate: " << std::get<0>(station.first) << ","
            << std::get<1>(station.first) << std::endl;
  
  auto vaporized = asteroids.vaporized(station.first, 200);
  std::cout << "The 200th asteroid to be vaporized is at " << std::get<0>(vaporized) << ","
            << std::get<1>(vaporized) << std::endl;
  
  return 0;
}