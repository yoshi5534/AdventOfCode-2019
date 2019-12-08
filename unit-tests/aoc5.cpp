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

TEST_CASE ("Support comparision")
{
  SECTION ("equal to position mode")
  {
    Program program {3,9,8,9,10,9,4,9,99,-1,8};
    {
      Computer computer;
      computer.writeInput ({8});
      computer.calculate (program);
      auto output = computer.readOutput ();
      REQUIRE (output == 1);
    }
    {
      Computer computer;
      computer.writeInput ({5});
      computer.calculate (program);
      auto output = computer.readOutput ();
      REQUIRE (output == 0);
    }
  }

  SECTION ("less than position mode")
  {
    Program program {3,9,7,9,10,9,4,9,99,-1,8};
    {
      Computer computer;
      computer.writeInput ({4});
      computer.calculate (program);
      auto output = computer.readOutput ();
      REQUIRE (output == 1);
    }
    {
      Computer computer;
      computer.writeInput ({15});
      computer.calculate (program);
      auto output = computer.readOutput ();
      REQUIRE (output == 0);
    }
  }

  SECTION ("equal to immediate mode")
  {
    Program program {3,3,1108,-1,8,3,4,3,99};
    {
      Computer computer;
      computer.writeInput ({8});
      computer.calculate (program);
      auto output = computer.readOutput ();
      REQUIRE (output == 1);
    }
    {
      Computer computer;
      computer.writeInput ({15});
      computer.calculate (program);
      auto output = computer.readOutput ();
      REQUIRE (output == 0);
    }
  }

  SECTION ("less than immediate mode")
  {
    Program program {3,3,1107,-1,8,3,4,3,99};
    {
      Computer computer;
      computer.writeInput ({7});
      computer.calculate (program);
      auto output = computer.readOutput ();
      REQUIRE (output == 1);
    }
    {
      Computer computer;
      computer.writeInput ({8});
      computer.calculate (program);
      auto output = computer.readOutput ();
      REQUIRE (output == 0);
    }
  }
}

TEST_CASE ("Support jumps")
{
  SECTION ("jump test position mode")
  {
    Program program {3,12,6,12,15,1,13,14,13,4,13,99,-1,0,1,9};
    {
      Computer computer;
      computer.writeInput ({0});
      computer.calculate (program);
      auto output = computer.readOutput ();
      REQUIRE (output == 0);
    }
    {
      Computer computer;
      computer.writeInput ({8});
      computer.calculate (program);
      auto output = computer.readOutput ();
      REQUIRE (output == 1);
    }
  }

  SECTION ("jump test immediate mode")
  {
    Program program {3,3,1105,-1,9,1101,0,0,12,4,12,99,1};
    {
      Computer computer;
      computer.writeInput ({0});
      computer.calculate (program);
      auto output = computer.readOutput ();
      REQUIRE (output == 0);
    }
    {
      Computer computer;
      computer.writeInput ({8});
      computer.calculate (program);
      auto output = computer.readOutput ();
      REQUIRE (output == 1);
    }
  }
}

TEST_CASE ("day 5 - puzzle 1")
{
  Program program {3,225,1,225,6,6,1100,1,238,225,104,0,2,106,196,224,101,-1157,224,224,4,224,102,8,223,223,1001,224,7,224,1,224,223,223,1002,144,30,224,1001,224,-1710,224,4,224,1002,223,8,223,101,1,224,224,1,224,223,223,101,82,109,224,1001,224,-111,224,4,224,102,8,223,223,1001,224,4,224,1,223,224,223,1102,10,50,225,1102,48,24,224,1001,224,-1152,224,4,224,1002,223,8,223,101,5,224,224,1,223,224,223,1102,44,89,225,1101,29,74,225,1101,13,59,225,1101,49,60,225,1101,89,71,224,1001,224,-160,224,4,224,1002,223,8,223,1001,224,6,224,1,223,224,223,1101,27,57,225,102,23,114,224,1001,224,-1357,224,4,224,102,8,223,223,101,5,224,224,1,224,223,223,1001,192,49,224,1001,224,-121,224,4,224,1002,223,8,223,101,3,224,224,1,223,224,223,1102,81,72,225,1102,12,13,225,1,80,118,224,1001,224,-110,224,4,224,102,8,223,223,101,2,224,224,1,224,223,223,4,223,99,0,0,0,677,0,0,0,0,0,0,0,0,0,0,0,1105,0,99999,1105,227,247,1105,1,99999,1005,227,99999,1005,0,256,1105,1,99999,1106,227,99999,1106,0,265,1105,1,99999,1006,0,99999,1006,227,274,1105,1,99999,1105,1,280,1105,1,99999,1,225,225,225,1101,294,0,0,105,1,0,1105,1,99999,1106,0,300,1105,1,99999,1,225,225,225,1101,314,0,0,106,0,0,1105,1,99999,7,677,226,224,102,2,223,223,1005,224,329,101,1,223,223,108,226,226,224,102,2,223,223,1006,224,344,101,1,223,223,1108,226,677,224,102,2,223,223,1006,224,359,1001,223,1,223,107,677,677,224,1002,223,2,223,1005,224,374,1001,223,1,223,1107,226,677,224,102,2,223,223,1005,224,389,1001,223,1,223,107,677,226,224,1002,223,2,223,1005,224,404,101,1,223,223,8,226,677,224,102,2,223,223,1005,224,419,101,1,223,223,7,226,677,224,1002,223,2,223,1005,224,434,101,1,223,223,1007,677,677,224,102,2,223,223,1006,224,449,1001,223,1,223,107,226,226,224,1002,223,2,223,1006,224,464,1001,223,1,223,1007,226,226,224,102,2,223,223,1006,224,479,1001,223,1,223,1008,226,226,224,102,2,223,223,1006,224,494,101,1,223,223,7,677,677,224,102,2,223,223,1005,224,509,1001,223,1,223,108,677,226,224,102,2,223,223,1005,224,524,101,1,223,223,1108,677,226,224,1002,223,2,223,1006,224,539,101,1,223,223,1108,677,677,224,102,2,223,223,1005,224,554,101,1,223,223,8,677,226,224,102,2,223,223,1005,224,569,101,1,223,223,8,677,677,224,102,2,223,223,1005,224,584,101,1,223,223,1107,226,226,224,102,2,223,223,1006,224,599,101,1,223,223,108,677,677,224,102,2,223,223,1006,224,614,101,1,223,223,1008,677,226,224,1002,223,2,223,1005,224,629,1001,223,1,223,1107,677,226,224,102,2,223,223,1005,224,644,101,1,223,223,1008,677,677,224,1002,223,2,223,1005,224,659,101,1,223,223,1007,677,226,224,1002,223,2,223,1005,224,674,1001,223,1,223,4,223,99,226};
  Computer computer;
  computer.writeInput ({1});
  computer.calculate (program);

  auto output = computer.readOutput ();
}