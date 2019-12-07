#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <wires.h>
using namespace AdventOfCode;

TEST_CASE ("Wire tracing", "[wires]")
{
  WirePath path_1 = fromString ("R8,U5,L5,D3");
  WirePath path_2 = fromString ("U7,R6,D4,L4");
  
  Intersections locations = intersects (path_1, path_2);

  REQUIRE (locations.size () == 3);

  int shortestDistance = distances (locations)[0];
  REQUIRE (shortestDistance == 6);
}