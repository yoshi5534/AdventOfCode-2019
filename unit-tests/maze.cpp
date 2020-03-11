#include "doctest.h"

#include <maze.h>

#include <sstream>

using namespace AdventOfCode;

TEST_CASE("Simple maze 1") {
  std::stringstream map;
  map << "         A           \n"
         "         A           \n"
         "  #######.#########  \n"
         "  #######.........#  \n"
         "  #######.#######.#  \n"
         "  #######.#######.#  \n"
         "  #######.#######.#  \n"
         "  #####  B    ###.#  \n"
         "BC...##  C    ###.#  \n"
         "  ##.##       ###.#  \n"
         "  ##...DE  F  ###.#  \n"
         "  #####    G  ###.#  \n"
         "  #########.#####.#  \n"
         "DE..#######...###.#  \n"
         "  #.#########.###.#  \n"
         "FG..#########.....#  \n"
         "  ###########.#####  \n"
         "             Z       \n"
         "             Z       \n";

  Maze maze(map);
  auto steps = maze.shortestPath();
  int expected = 23;
  REQUIRE(steps == expected);
}