#include <catch2/catch.hpp>

#include <motion.h>
using namespace AdventOfCode;

TEST_CASE("Four moons") {
  Motion simulation;

  simulation.addMoon("<x=-1, y=0, z=2>");
  simulation.addMoon("<x=2, y=-10, z=-7>");
  simulation.addMoon("<x=4, y=-8, z=8>");
  simulation.addMoon("<x=3, y=5, z=-1>");

  for (int i = 0; i < 10; ++i) {
    simulation.timestep();
  }

  int expected = 179;
  REQUIRE(expected == simulation.totalEnergy());
}

TEST_CASE("Four other moons") {
  Motion simulation;

  simulation.addMoon("<x=-8, y=-10, z=0>");
  simulation.addMoon("<x=5, y=5, z=10>");
  simulation.addMoon("<x=2, y=-7, z=3>");
  simulation.addMoon("<x=9, y=-8, z=-3>");

  for (int i = 0; i < 100; ++i) {
    simulation.timestep();
  }

  int expected = 1940;
  REQUIRE(expected == simulation.totalEnergy());
}

TEST_CASE("AOC_12") {
  Motion simulation;

  simulation.addMoon("<x=3, y=2, z=-6>");
  simulation.addMoon("<x=-13, y=18, z=10>");
  simulation.addMoon("<x=-8, y=-1, z=13>");
  simulation.addMoon("<x=5, y=10, z=4>");

  for (int i = 0; i < 1000; ++i) {
    simulation.timestep();
  }

  int expected = 14780;
  REQUIRE(expected == simulation.totalEnergy());
}