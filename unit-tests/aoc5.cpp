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
      Computer::calculate (program);

      THEN ("the output should be the same")
      {

      }
    }
  }
}