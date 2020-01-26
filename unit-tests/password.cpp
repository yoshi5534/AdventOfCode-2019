#include "doctest.h"

#include <password.h>

using namespace AdventOfCode;

TEST_CASE("Passwords may be special") {
  REQUIRE(false == isValidPassword(111111));
  REQUIRE(false == isValidPassword(123789));
  REQUIRE(false == isValidPassword(223450));
  REQUIRE(false == isValidPassword(123444));
  REQUIRE(true == isValidPassword(111122));
  REQUIRE(true == isValidPassword(111223));
  REQUIRE(true == isValidPassword(112233));
  REQUIRE(true == isValidPassword(122333));
  REQUIRE(false == isValidPassword(222333));
}