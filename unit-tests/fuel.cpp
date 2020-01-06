#include <catch2/catch.hpp>

#include <fuel.h>

using namespace AdventOfCode;

TEST_CASE("Easy FUEL 1") {
  NanoFactory factory;

  factory.addReaction("10 ORE => 10 A");
  factory.addReaction("1 ORE => 1 B");
  factory.addReaction("7 A, 1 B => 1 C");
  factory.addReaction("7 A, 1 C => 1 D");
  factory.addReaction("7 A, 1 D => 1 E");
  factory.addReaction("7 A, 1 E => 1 FUEL");

  int expected = 31;
  REQUIRE(expected == factory.necessaryORE());
}

TEST_CASE("Easy FUEL 2") {
  NanoFactory factory;

  factory.addReaction("9 ORE => 2 A");
  factory.addReaction("8 ORE => 3 B");
  factory.addReaction("7 ORE => 5 C");
  factory.addReaction("3 A, 4 B => 2 AB");
  factory.addReaction("5 B, 7 C => 1 BC");
  factory.addReaction("4 C, 1 A => 1 CA");
  factory.addReaction("5 AB, 3 BC, 4 CA => 1 FUEL");

  int expected = 8 * 7 + 9 * 8 + 7 * 9;
  REQUIRE(expected == factory.necessaryORE());
}

// TEST_CASE("More complex FUEL 1") {
//   NanoFactory factory;

//   factory.addReaction("157 ORE => 5 NZVS");
//   factory.addReaction("165 ORE => 6 DCFZ");
//   factory.addReaction(
//       "44 XJWVT, 5 KHKGT, 1 QDVJ, 29 NZVS, 9 GPVTF, 48 HKGWZ => 1 FUEL");
//   factory.addReaction("12 HKGWZ, 1 GPVTF, 8 PSHF => 9 QDVJ");
//   factory.addReaction("179 ORE => 7 PSHF");
//   factory.addReaction("177 ORE => 5 HKGWZ");
//   factory.addReaction("7 DCFZ, 7 PSHF => 2 XJWVT");
//   factory.addReaction("165 ORE => 2 GPVTF");
//   factory.addReaction("3 DCFZ, 7 NZVS, 5 HKGWZ, 10 PSHF => 8 KHKGT");

//   int expected = 13312;
//   REQUIRE(expected == factory.necessaryORE());
// }