#include <catch2/catch.hpp>

#include <computer.h>
using namespace AdventOfCode;


SCENARIO( "computer can handle input and output" )
{
  GIVEN( "A program with new Intcode commands 3 and 4" )
  {
    WHEN ("a specific value is given")
    {
      Program program {3,0,4,0,99};
      Computer computer;
      computer.writeInput ({1});
      computer.calculate (program);

      THEN ("the output should be the same")
      {
        Memory expected = {1,0,4,0,99};
        auto result = computer.readMemory ();
        REQUIRE (result == expected);

        auto output = computer.readOutput ();
        REQUIRE (output == 1);
      }
    }
  }
}