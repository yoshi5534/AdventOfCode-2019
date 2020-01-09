#include <catch2/catch.hpp>

#include <fft.h>

#include <iostream>

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

TEST_CASE("Convert from string to InputSignal") {
  std::string const input{"12345678"};
  InputSignal signal = FFT::fromString(input, 1);

  InputSignal expected{1, 2, 3, 4, 5, 6, 7, 8};
  REQUIRE(expected == signal);
}

TEST_CASE("FFT simple input") {
  InputSignal input{1, 2, 3, 4, 5, 6, 7, 8};

  OutputSignal phase1 = FFT::outputSignal(input);
  OutputSignal expected_1{4, 8, 2, 2, 6, 1, 5, 8};

  REQUIRE(phase1 == expected_1);
}

TEST_CASE("FFT longer input") {
  auto signal = FFT::fromString("80871224585914546619083218645595", 1);
  for (int i = 0; i < 100; ++i) {
    signal = FFT::outputSignal(signal);
  }

  auto result =
      OutputSignal{std::begin(signal), std::next(std::begin(signal), 8)};
  auto expected = FFT::fromString("24176176", 1);

  REQUIRE(expected == result);
}

TEST_CASE("FFT longer input with many repetitions") {
  std::string const input{"03036732577212944063491565474664"};
  auto signal = FFT::fromString(input, 300);
  auto offset = std::stol(
      std::string{std::begin(input), std::next(std::begin(input), 7)});

  for (int i = 0; i < 100; ++i) {
    std::cout << ".";
    signal = FFT::outputSignal(signal);
  }

  auto result =
      OutputSignal{std::next(std::begin(signal), 0), std::next(std::begin(signal), 0 + 8)};
  auto expected = FFT::fromString("84462026", 1);

  REQUIRE(expected == result);
}

  // 123456789 123456789 123456789 123456789 123456789 123456789 123456789
  // 01100AA00 1100AA001 100AA0011 00AA00110 0AA001100 AA001100A A001100AA 001100AA0 01100AA00
  // 00111000A AA0001110 00AAA0001 11000AAA0 00111000A AA0001110 00AAA0001
  // 000111100 00AAAA000 011110000 AAAA00001 1110000AA AA0000111 10000AAAA
  // 000011111 00000AAAA A00000111 1100000AA AAA000001 111100000 AAAAA0000
  // 000001111 11000000A AAAAA0000 001111110 00000AAAA AA0000001 111110000
