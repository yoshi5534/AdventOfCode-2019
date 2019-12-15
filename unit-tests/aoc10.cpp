#include <catch2/catch.hpp>

#include <asteroids.h>
#include <map>
#include <sstream>
using namespace AdventOfCode;

TEST_CASE("Remove line of asteroids") {
  AsteroidMap map{{1, 0}, {4, 0}, {0, 2}, {1, 2}, {2, 2},
                  {3, 2}, {4, 2}, {4, 3}, {3, 4}, {4, 4}};
  int width = 5;
  int height = 5;

  Coordinates start{3, 4};
  Coordinates offset{-1, -2};

  Asteroids asteroids{map, width, height};
  REQUIRE(asteroids.count() == 10);
  asteroids.print();

  asteroids.removeLine(start, offset);
  REQUIRE(asteroids.count() == 8);
}

TEST_CASE("First example") {
  AsteroidMap map{{1, 0}, {4, 0}, {0, 2}, {1, 2}, {2, 2},
                  {3, 2}, {4, 2}, {4, 3}, {3, 4}, {4, 4}};
  int width = 5;
  int height = 5;
  Asteroids asteroids{map, width, height};

  int expected{8};
  auto mostVisible = asteroids.mostVisible();
  REQUIRE(mostVisible == expected);
}

TEST_CASE("Read map") {
  std::string textMap = "......#.#. "
                        "#..#.#.... "
                        "..#######. "
                        ".#.#.###.. "
                        ".#..#..... "
                        "..#....#.# "
                        "#..#....#. "
                        ".##.#..### "
                        "##...#..#. "
                        ".#....#### ";

  std::istringstream mapstream{textMap};
  std::string line;

  AsteroidMap map;
  int height = 0;
  int width = 0;
  while (std::getline(mapstream, line, ' ')) {
    width = 0;
    for (char c : line) {
      if (c == '#')
        map.insert({width, height});
      width++;
    }
    height++;
  }

  Asteroids asteroids{map, width, height};
  int expected{33};
  auto mostVisible = asteroids.mostVisible();
  REQUIRE(mostVisible == expected);
}