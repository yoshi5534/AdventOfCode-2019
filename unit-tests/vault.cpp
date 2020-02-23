#include "doctest.h"

#include <sstream>
#include <vault.h>

using namespace AdventOfCode;

TEST_CASE("Collect all keys") {
  std::stringstream map;
  map << "#########\n"
         "#b.A.@.a#\n"
         "#########\n";

  Vault vault{map};
  auto steps = vault.collectKeys ();
  int expected = 8;
  REQUIRE(expected == steps);
}