#include <catch2/catch.hpp>

#include <asteroids.h>
#include <map>
#include <sstream>
using namespace AdventOfCode;


TEST_CASE("First example") {
  AsteroidMap map{{1, 0}, {4, 0}, {0, 2}, {1, 2}, {2, 2},
                  {3, 2}, {4, 2}, {4, 3}, {3, 4}, {4, 4}};
  int width = 5;
  int height = 5;
  Asteroids asteroids{map, width, height};

  int expected{8};
  auto mostVisible = asteroids.mostVisible();
  REQUIRE(mostVisible.first == expected);
}
namespace {
Asteroids getAsteroids(std::string const &textMap) {

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

  return Asteroids{map, width, height};
}
} // namespace

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

  int expected{33};
  auto map = getAsteroids(textMap);
  REQUIRE(map.mostVisible().first == expected);
}

TEST_CASE("Bigger example") {
  std::string textMap = ".#..##.###...####### "
                        "##.############..##. "
                        ".#.######.########.# "
                        ".###.#######.####.#. "
                        "#####.##.#.##.###.## "
                        "..#####..#.######### "
                        "#################### "
                        "#.####....###.#.#.## "
                        "##.################# "
                        "#####.##.###..####.. "
                        "..######..##.####### "
                        "####.##.####...##..# "
                        ".#####..#.######.### "
                        "##...#.##########... "
                        "#.##########.####### "
                        ".####.#.###.###.#.## "
                        "....##.##.###..##### "
                        ".#.#.###########.### "
                        "#.#.#.#####.####.### "
                        "###.##.####.##.#..## ";

  auto map = getAsteroids(textMap);
  {
    int expected{210};
    REQUIRE(map.mostVisible().first == expected);
  }
  {
    auto expected = Coordinates{8, 2};
    auto destroyed = map.vaporized({11, 13}, 200);
    REQUIRE(destroyed == expected);
  }
}