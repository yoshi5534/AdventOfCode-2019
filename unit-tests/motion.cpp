#include <catch2/catch.hpp>

#include <motion.h>
using namespace AdventOfCode;

TEST_CASE("Four moons") {
  Motion simulation;

  simulation.addMoon("<x=-1, y=0, z=2>");
  simulation.addMoon("<x=2, y=-10, z=-7>");
  simulation.addMoon("<x=4, y=-8, z=8>");
  simulation.addMoon("<x=3, y=5, z=-1>");

  {
    auto repeatSimulation = simulation;
    REQUIRE(2772 == repeatSimulation.repeatingTime());
  }

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
  {
    auto repeatSimulation = simulation;
    REQUIRE(4686774924L == repeatSimulation.repeatingTime());
  }

  for (int i = 0; i < 100; ++i) {
    simulation.timestep();
  }

  int expected = 1940;
  REQUIRE(expected == simulation.totalEnergy());
}