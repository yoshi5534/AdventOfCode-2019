#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <computer.h>
using namespace AdventOfCode;

SCENARIO( "computer can handle programs" )
{
  GIVEN( "A program with a valid Intcode command" )
  {
    Computer::Program program {1,0,0,3,99};

    WHEN ("the computer calculates the output")
    {
      auto const output = Computer::calculate (program);

      THEN ("the output is correct")
      {
        Computer::Program correct = {1,0,0,2,99};
        REQUIRE (output == correct);
      }
    }
  }
}