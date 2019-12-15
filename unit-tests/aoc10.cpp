#include <catch2/catch.hpp>

#include <asteroids.h>
#include <map>
using namespace AdventOfCode;

TEST_CASE("Find asteroids") {
  AsteroidMap map{{1, 0}, {4, 0}, {0, 2}, {1, 2}, {2, 2},
                  {3, 2}, {4, 2}, {4, 3}, {3, 4}, {4, 4}};
  int width = 5;
  int height = 5;

  std::map<Coordinates, int> visibles;

  std::for_each(std::begin(map), std::end(map), [&](auto candidate) {
    visibles[candidate] = 0;
    Asteroids asteroids{map, width, height};
    for (int x = 0; x < width; ++x) {
      for (int y = 0; y < height; ++y) {
        if (Coordinates{x, y} == Coordinates{0, 0})
          continue;
        {
          Coordinates offset{-1 * x, -1 * y};
          if (asteroids.isInMap(candidate + offset)) {
            auto line = asteroids.getLineCoords(candidate, offset);
            if (std::any_of(std::begin(line), std::end(line),
                            [&asteroids](auto coords) {
                              return asteroids.hasAsteroid(coords);
                            })) {
              visibles[candidate]++;
              asteroids.removeLine(candidate, offset);
            }
          }
        }
        {
          Coordinates offset{-1 * x, y};
          if (asteroids.isInMap(candidate + offset)) {
            auto line = asteroids.getLineCoords(candidate, offset);
            if (std::any_of(std::begin(line), std::end(line),
                            [&asteroids](auto coords) {
                              return asteroids.hasAsteroid(coords);
                            })) {
              visibles[candidate]++;
              asteroids.removeLine(candidate, offset);
            }
          }
        }
        {
          Coordinates offset{x, -1 * y};
          if (asteroids.isInMap(candidate + offset)) {
            auto line = asteroids.getLineCoords(candidate, offset);
            if (std::any_of(std::begin(line), std::end(line),
                            [&asteroids](auto coords) {
                              return asteroids.hasAsteroid(coords);
                            })) {
              visibles[candidate]++;
              asteroids.removeLine(candidate, offset);
            }
          }
        }
        {
          Coordinates offset{x, y};
          if (asteroids.isInMap(candidate + offset)) {
            auto line = asteroids.getLineCoords(candidate, offset);
            if (std::any_of(std::begin(line), std::end(line),
                            [&asteroids](auto coords) {
                              return asteroids.hasAsteroid(coords);
                            })) {
              visibles[candidate]++;
              asteroids.removeLine(candidate, offset);
            }
          }
        }
      }
    }
  });

  Coordinates start{3, 4};
  Coordinates offset{-1, -2};

  Asteroids asteroids{map, width, height};
  REQUIRE(asteroids.count() == 10);
  asteroids.print();

  asteroids.removeLine(start, offset);
  REQUIRE(asteroids.count() == 7);
}