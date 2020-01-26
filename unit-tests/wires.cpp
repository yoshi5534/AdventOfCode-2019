#include "doctest.h"

#include <wires.h>
using namespace AdventOfCode;

TEST_CASE("Wire tracing") {
  WirePath path_1 = fromString("R8,U5,L5,D3");
  WirePath path_2 = fromString("U7,R6,D4,L4");

  Intersections locations = intersects(path_1, path_2);

  REQUIRE(locations.size() == 3);

  int shortestDistance = distances(locations)[0];
  REQUIRE(shortestDistance == 6);

  int steps = minimalSignalDelay(path_1, path_2, locations);
  REQUIRE(steps == 30);
}

TEST_CASE("Wire tracing 2") {
  WirePath path_1 = fromString("R75,D30,R83,U83,L12,D49,R71,U7,L72");
  WirePath path_2 = fromString("U62,R66,U55,R34,D71,R55,D58,R83");

  Intersections locations = intersects(path_1, path_2);

  int shortestDistance = distances(locations)[0];
  REQUIRE(shortestDistance == 159);

  int steps = minimalSignalDelay(path_1, path_2, locations);
  REQUIRE(steps == 610);
}