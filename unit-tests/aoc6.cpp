#include <catch2/catch.hpp>

#include <orbit.h>
using namespace AdventOfCode;

TEST_CASE("Small map") {
  Map map;
  auto entries = {
    "COM)B",
    "B)C",
    "C)D",
    "D)E",
    "E)F",
    "B)G",
    "G)H",
    "D)I",
    "E)J",
    "J)K",
    "K)L"
  };

  for (auto const& entry: entries)
  {
    map.addOrbit (entry);
  }

  REQUIRE ( map.checksum () == 42);
}