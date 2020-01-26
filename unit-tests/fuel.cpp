#include "doctest.h"

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
  factory.addReaction("3 A, 4 B => 1 AB");
  factory.addReaction("5 B, 7 C => 1 BC");
  factory.addReaction("4 C, 1 A => 1 CA");
  factory.addReaction("2 AB, 3 BC, 4 CA => 1 FUEL");

  int expected = 165;
  REQUIRE(expected == factory.necessaryORE());
}

TEST_CASE("More complex FUEL 1") {
  NanoFactory factory;

  factory.addReaction("157 ORE => 5 NZVS");
  factory.addReaction("165 ORE => 6 DCFZ");
  factory.addReaction(
      "44 XJWVT, 5 KHKGT, 1 QDVJ, 29 NZVS, 9 GPVTF, 48 HKGWZ => 1 FUEL");
  factory.addReaction("12 HKGWZ, 1 GPVTF, 8 PSHF => 9 QDVJ");
  factory.addReaction("179 ORE => 7 PSHF");
  factory.addReaction("177 ORE => 5 HKGWZ");
  factory.addReaction("7 DCFZ, 7 PSHF => 2 XJWVT");
  factory.addReaction("165 ORE => 2 GPVTF");
  factory.addReaction("3 DCFZ, 7 NZVS, 5 HKGWZ, 10 PSHF => 8 KHKGT");

  int64_t expected = 13312;
  REQUIRE(expected == factory.necessaryORE());
}

TEST_CASE("More complex FUEL 2") {
  NanoFactory factory;

  factory.addReaction("2 VPVL, 7 FWMGM, 2 CXFTF, 11 MNCFX => 1 STKFG");
  factory.addReaction("17 NVRVD, 3 JNWZP => 8 VPVL");
  factory.addReaction(
      "53 STKFG, 6 MNCFX, 46 VJHF, 81 HVMC, 68 CXFTF, 25 GNMV => 1 FUEL");
  factory.addReaction("22 VJHF, 37 MNCFX => 5 FWMGM");
  factory.addReaction("139 ORE => 4 NVRVD");
  factory.addReaction("144 ORE => 7 JNWZP");
  factory.addReaction("5 MNCFX, 7 RFSQX, 2 FWMGM, 2 VPVL, 19 CXFTF => 3 HVMC");
  factory.addReaction("5 VJHF, 7 MNCFX, 9 VPVL, 37 CXFTF => 6 GNMV");
  factory.addReaction("145 ORE => 6 MNCFX");
  factory.addReaction("1 NVRVD => 8 CXFTF");
  factory.addReaction("1 VJHF, 6 MNCFX => 4 RFSQX");
  factory.addReaction("176 ORE => 6 VJHF");

  int expected = 180697;
  REQUIRE(expected == factory.necessaryORE());
}

TEST_CASE("More complex FUEL 3") {
  NanoFactory factory;

 factory.addReaction("171 ORE => 8 CNZTR");
 factory.addReaction("7 ZLQW, 3 BMBT, 9 XCVML, 26 XMNCP, 1 WPTQ, 2 MZWV, 1 RJRHP => 4 PLWSL");
 factory.addReaction("114 ORE => 4 BHXH");
 factory.addReaction("14 VRPVC => 6 BMBT");
 factory.addReaction("6 BHXH, 18 KTJDG, 12 WPTQ, 7 PLWSL, 31 FHTLT, 37 ZDVW => 1 FUEL");
 factory.addReaction("6 WPTQ, 2 BMBT, 8 ZLQW, 18 KTJDG, 1 XMNCP, 6 MZWV, 1 RJRHP => 6 FHTLT");
 factory.addReaction("15 XDBXC, 2 LTCX, 1 VRPVC => 6 ZLQW");
 factory.addReaction("13 WPTQ, 10 LTCX, 3 RJRHP, 14 XMNCP, 2 MZWV, 1 ZLQW => 1 ZDVW");
 factory.addReaction("5 BMBT => 4 WPTQ");
 factory.addReaction("189 ORE => 9 KTJDG");
 factory.addReaction("1 MZWV, 17 XDBXC, 3 XCVML => 2 XMNCP");
 factory.addReaction("12 VRPVC, 27 CNZTR => 2 XDBXC");
 factory.addReaction("15 KTJDG, 12 BHXH => 5 XCVML");
 factory.addReaction("3 BHXH, 2 VRPVC => 7 MZWV");
 factory.addReaction("121 ORE => 7 VRPVC");
 factory.addReaction("7 XCVML => 6 RJRHP");
 factory.addReaction("5 BHXH, 4 VRPVC => 5 LTCX");

  int expected = 2210736;
  REQUIRE(expected == factory.necessaryORE());

  int64_t expectedFuel = 460664;
  // REQUIRE(expectedFuel == factory.maximumFuel(1000000000000));
}