#include <catch2/catch.hpp>

#include <computer.h>
using namespace AdventOfCode;

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