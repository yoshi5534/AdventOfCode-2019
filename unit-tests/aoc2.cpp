#include <catch2/catch.hpp>

#include <computer.h>
using namespace AdventOfCode;

SCENARIO( "computer can handle programs" )
{
  GIVEN( "A program with a valid Intcode command" )
  {
    WHEN ("the program should add 1 and 1")
    {
      Program program {1,0,0,3,99};
      Computer computer;
      computer.calculate (program);

      THEN ("the output should be 2")
      {
        Memory expected = {1,0,0,2,99};
        auto result = computer.readMemory ();
        REQUIRE (result == expected);
      }
    }

    WHEN ("the program should multiply 2 and 2")
    {
      Program program {2,0,0,3,99};
      Computer computer;
      computer.calculate (program);

      THEN ("the output should be 4")
      {
        Memory expected = {2,0,0,4,99};
        auto result = computer.readMemory ();
        REQUIRE (result == expected);
      }
    }
  }

  GIVEN ("A program with multiple command")
  {
    WHEN ("the halt command is at the end")
    {
      Program program {2,3,0,3,2,8,8,9,99,0};
      Computer computer;
      computer.calculate (program);

      THEN ("all calculations should be done")
      {
        Memory expected = {2,3,0,6,2,8,8,9,99,9801};
        auto result = computer.readMemory ();
        REQUIRE (result == expected);
      }
    }

    WHEN ("the halt command is in the middle")
    {
      Program program {2,3,0,3,99,2,8,8,9,99,0};
      Computer computer;
      computer.calculate (program);

      THEN ("only the first calculation should be done")
      {
        Memory expected = {2,3,0,6,99,2,8,8,9,99,0};
        auto result = computer.readMemory ();
        REQUIRE (result == expected);
      }
    }
  }
}