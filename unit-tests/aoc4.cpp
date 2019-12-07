#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <password.h>


using namespace AdventOfCode;

TEST_CASE ("Passwords may be special")
{
  REQUIRE (true == isValidPassword (111111));
  REQUIRE (false == isValidPassword (123789));
  REQUIRE (false == isValidPassword (223450));
}
