#include <catch2/catch.hpp>

#include <amplifier.h>
using namespace AdventOfCode;


TEST_CASE("Thrust example 1") {
  Program program {3,15,3,16,1002,16,10,16,1,16,15,15,4,15,99,0,0};
  int expected = 43210;

  REQUIRE (AmplifierChain::maxThrusterSignal (program) == expected);
}