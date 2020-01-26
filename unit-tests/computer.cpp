#include "doctest.h"

#include <computer.h>

#include <algorithm>
using namespace AdventOfCode;

bool memoryComparsion (Memory const& left, Memory const& right)
{
  Memory memoryBegin (right.size ());
  std::copy (std::begin (left), std::begin (left) + right.size (), std::begin (memoryBegin));

  return memoryBegin == right;
}

SCENARIO("computer can handle programs") {
  GIVEN("A program with a valid Intcode command") {
    WHEN("the program should add 1 and 1") {
      Program program{1, 0, 0, 3, 99};
      Computer computer(program);
      computer.calculate();

      THEN("the output should be 2") {
        Memory expected = {1, 0, 0, 2, 99};
        auto result = computer.accessMemory();
        REQUIRE(memoryComparsion(result, expected));
      }
    }

    WHEN("the program should multiply 2 and 2") {
      Program program{2, 0, 0, 3, 99};
      Computer computer(program);
      computer.calculate();

      THEN("the output should be 4") {
        Memory expected = {2, 0, 0, 4, 99};
        auto result = computer.accessMemory();
        REQUIRE(memoryComparsion(result, expected));
      }
    }
  }

  GIVEN("A program with multiple command") {
    WHEN("the halt command is at the end") {
      Program program{2, 3, 0, 3, 2, 8, 8, 9, 99, 0};
      Computer computer(program);
      computer.calculate();

      THEN("all calculations should be done") {
        Memory expected = {2, 3, 0, 6, 2, 8, 8, 9, 99, 9801};
        auto result = computer.accessMemory();
        REQUIRE(memoryComparsion(result, expected));
      }
    }

    WHEN("the halt command is in the middle") {
      Program program{2, 3, 0, 3, 99, 2, 8, 8, 9, 99, 0};
      Computer computer(program);
      computer.calculate();

      THEN("only the first calculation should be done") {
        Memory expected = {2, 3, 0, 6, 99, 2, 8, 8, 9, 99, 0};
        auto result = computer.accessMemory();
        REQUIRE(memoryComparsion(result, expected));
      }
    }
  }
}

SCENARIO("computer can handle input and output") {
  GIVEN("A program with new Intcode commands 3 and 4") {
    WHEN("a specific value is given") {
      Program program{3, 0, 4, 0, 99};
      Computer computer(program);
      computer.writeInput({1});
      computer.calculate();

      THEN("the output should be the same") {
        Memory expected = {1, 0, 4, 0, 99};
        auto result = computer.accessMemory();
        REQUIRE(memoryComparsion(result, expected));

        auto output = computer.readOutput();
        REQUIRE(output == 1);
      }
    }
  }
}

SCENARIO("Parameters can have different modes") {
  GIVEN("A program with different parameter modes") {
    WHEN("the operation is performed") {
      Program program{1002, 4, 3, 4, 33};
      Computer computer(program);
      computer.calculate();

      THEN("the parameters are interpreted correctly") {
        Memory expected = {1002, 4, 3, 4, 99};
        auto result = computer.accessMemory();
        REQUIRE(memoryComparsion(result, expected));
      }
    }
  }
}

SCENARIO("Parameters can be negative") {
  GIVEN("A program with negative parameters") {
    WHEN("the operation is calculated") {
      Program program{1101, 100, -1, 4, 0};
      Computer computer(program);
      computer.calculate();

      THEN("a substraction is performed") {
        Memory expected = {1101, 100, -1, 4, 99};
        auto result = computer.accessMemory();
        REQUIRE(memoryComparsion(result, expected));
      }
    }
  }
}

TEST_CASE("Support comparision") {
  SUBCASE("equal to position mode") {
    Program program{3, 9, 8, 9, 10, 9, 4, 9, 99, -1, 8};
    {
      Computer computer(program);
      computer.writeInput({8});
      computer.calculate();
      auto output = computer.readOutput();
      REQUIRE(output == 1);
    }
    {
      Computer computer(program);
      computer.writeInput({5});
      computer.calculate();
      auto output = computer.readOutput();
      REQUIRE(output == 0);
    }
  }

  SUBCASE("less than position mode") {
    Program program{3, 9, 7, 9, 10, 9, 4, 9, 99, -1, 8};
    {
      Computer computer(program);
      computer.writeInput({4});
      computer.calculate();
      auto output = computer.readOutput();
      REQUIRE(output == 1);
    }
    {
      Computer computer(program);
      computer.writeInput({15});
      computer.calculate();
      auto output = computer.readOutput();
      REQUIRE(output == 0);
    }
  }

  SUBCASE("equal to immediate mode") {
    Program program{3, 3, 1108, -1, 8, 3, 4, 3, 99};
    {
      Computer computer(program);
      computer.writeInput({8});
      computer.calculate();
      auto output = computer.readOutput();
      REQUIRE(output == 1);
    }
    {
      Computer computer(program);
      computer.writeInput({15});
      computer.calculate();
      auto output = computer.readOutput();
      REQUIRE(output == 0);
    }
  }

  SUBCASE("less than immediate mode") {
    Program program{3, 3, 1107, -1, 8, 3, 4, 3, 99};
    {
      Computer computer(program);
      computer.writeInput({7});
      computer.calculate();
      auto output = computer.readOutput();
      REQUIRE(output == 1);
    }
    {
      Computer computer(program);
      computer.writeInput({8});
      computer.calculate();
      auto output = computer.readOutput();
      REQUIRE(output == 0);
    }
  }
}

TEST_CASE("Support jumps") {
  SUBCASE("jump test position mode") {
    Program program{3, 12, 6, 12, 15, 1, 13, 14, 13, 4, 13, 99, -1, 0, 1, 9};
    {
      Computer computer(program);
      computer.writeInput({0});
      computer.calculate();
      auto output = computer.readOutput();
      REQUIRE(output == 0);
    }
    {
      Computer computer(program);
      computer.writeInput({8});
      computer.calculate();
      auto output = computer.readOutput();
      REQUIRE(output == 1);
    }
  }

  SUBCASE("jump test immediate mode") {
    Program program{3, 3, 1105, -1, 9, 1101, 0, 0, 12, 4, 12, 99, 1};
    {
      Computer computer(program);
      computer.writeInput({0});
      computer.calculate();
      auto output = computer.readOutput();
      REQUIRE(output == 0);
    }
    {
      Computer computer(program);
      computer.writeInput({8});
      computer.calculate();
      auto output = computer.readOutput();
      REQUIRE(output == 1);
    }
  }
}

TEST_CASE("longer example") {
  Program program{3,  21,  1008, 21,   8,   20, 1005, 20,   22,   107,
                  8,  21,  20,   1006, 20,  31, 1106, 0,    36,   98,
                  0,  0,   1002, 21,   125, 20, 4,    20,   1105, 1,
                  46, 104, 999,  1105, 1,   46, 1101, 1000, 1,    20,
                  4,  20,  1105, 1,    46,  98, 99};

  SUBCASE("input lower 8") {
    Computer computer(program);
    computer.writeInput({0});
    computer.calculate();
    auto output = computer.readOutput();
    REQUIRE(output == 999);
  }

  SUBCASE("input equal 8") {
    Computer computer(program);
    computer.writeInput({8});
    computer.calculate();
    auto output = computer.readOutput();
    REQUIRE(output == 1000);
  }

  SUBCASE("input greater 8") {
    Computer computer(program);
    computer.writeInput({9});
    computer.calculate();
    auto output = computer.readOutput();
    REQUIRE(output == 1001);
  }
}

TEST_CASE("Copy of itself") {
  Program program{109,  1,   204, -1,  1001, 100, 1, 100,
                  1008, 100, 16,  101, 1006, 101, 0, 99};
  Computer computer(program);
  computer.calculate();

  for (auto code : program)
    REQUIRE(computer.readOutput() == code);
}

TEST_CASE("16 digit number") {
  Program program{1102, 34915192, 34915192, 7, 4, 7, 99, 0};
  Computer computer(program);
  computer.calculate();

  int64_t expected = int64_t(34915192) * int64_t(34915192);
  auto out = computer.readOutput();

  REQUIRE(expected == out);
}

TEST_CASE("really large") {
  Program program{104, 1125899906842624, 99};
  Computer computer(program);
  computer.calculate();

  int64_t expected = 1125899906842624;
  auto out = computer.readOutput();

  REQUIRE(expected == out);
}

TEST_CASE("failing isolated") {
 Program program{109,  20, 21102, 8, 9, -5, 4, 15, 99};
  Computer computer(program);
  computer.calculate();

  int64_t expected = 72;
  auto out = computer.readOutput();

  REQUIRE(expected == out);
}