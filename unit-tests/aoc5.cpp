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


SCENARIO( "Parameters can have different modes" )
{
  GIVEN( "A program with different parameter modes" )
  {
    WHEN ("the operation is performed")
    {
      Program program {1002,4,3,4,33};
      Computer computer;
      computer.calculate (program);

      THEN ("the parameters are interpreted correctly")
      {
        Memory expected = {1002,4,3,4,99};
        auto result = computer.readMemory ();
        REQUIRE (result == expected);
      }
    }
  }
}

SCENARIO( "Parameters can be negative" )
{
  GIVEN( "A program with negative parameters" )
  {
    WHEN ("the operation is calculated")
    {
      Program program {1101,100,-1,4,0};
      Computer computer;
      computer.calculate (program);

      THEN ("a substraction is performed")
      {
        Memory expected = {1101,100,-1,4,99};
        auto result = computer.readMemory ();
        REQUIRE (result == expected);
      }
    }
  }
}