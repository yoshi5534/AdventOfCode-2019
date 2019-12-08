#include <catch2/catch.hpp>

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

TEST_CASE("Find valid passwords") {
  int validPasswords = 0;

  for (int password = 231832; password <= 767346; ++password)
    if (isValidPassword(password))
      validPasswords++;

  REQUIRE(876 == validPasswords);
}