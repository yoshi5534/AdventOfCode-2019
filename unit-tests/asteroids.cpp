#include <catch2/catch.hpp>

#include <asteroids.h>
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

TEST_CASE("Read map") {
  std::stringstream textMap;
  textMap << "......#.#.\n"
             "#..#.#....\n"
             "..#######.\n"
             ".#.#.###..\n"
             ".#..#.....\n"
             "..#....#.#\n"
             "#..#....#.\n"
             ".##.#..###\n"
             "##...#..#.\n"
             ".#....####\n";

  int expected{33};
  Asteroids map{textMap};
  REQUIRE(map.mostVisible().first == expected);
}

TEST_CASE("Bigger example") {
  std::stringstream textMap;
  textMap << ".#..##.###...#######\n"
             "##.############..##.\n"
             ".#.######.########.#\n"
             ".###.#######.####.#.\n"
             "#####.##.#.##.###.##\n"
             "..#####..#.#########\n"
             "####################\n"
             "#.####....###.#.#.##\n"
             "##.#################\n"
             "#####.##.###..####..\n"
             "..######..##.#######\n"
             "####.##.####...##..#\n"
             ".#####..#.######.###\n"
             "##...#.##########...\n"
             "#.##########.#######\n"
             ".####.#.###.###.#.##\n"
             "....##.##.###..#####\n"
             ".#.#.###########.###\n"
             "#.#.#.#####.####.###\n"
             "###.##.####.##.#..##\n";

  Asteroids map{textMap};
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