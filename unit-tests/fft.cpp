#include <catch2/catch.hpp>

#include <fft.h>

using namespace AdventOfCode;

TEST_CASE("FFT pattern generator") {
  {
    auto pattern = PatternGenerator::get(8, 0);
    REQUIRE(pattern[2] == -1);
  }

  {
    auto pattern = PatternGenerator::get(8, 1);
    REQUIRE(pattern[2] == 1);
  }

  {
    auto pattern = PatternGenerator::get(8, 2);
    REQUIRE(pattern[2] == 1);
  }

  {
    auto pattern = PatternGenerator::get(8, 3);
    REQUIRE(pattern[2] == 0);
  }

  {
    auto pattern = PatternGenerator::get(83, 3);
    REQUIRE(pattern[2] == 0);
  }
}

TEST_CASE("FFT simple input") {
  InputSignal input{1, 2, 3, 4, 5, 6, 7, 8};

  REQUIRE(FFT::output(input, 0) == 4);
  REQUIRE(FFT::output(input, 1) == 8);

  OutputSignal phase1 = FFT::outputSignal(input);
  OutputSignal expected_1{4, 8, 2, 2, 6, 1, 5, 8};

  REQUIRE(phase1 == expected_1);
}