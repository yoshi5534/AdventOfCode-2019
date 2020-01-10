#include <catch2/catch.hpp>

#include <fft.h>

#include <iostream>

using namespace AdventOfCode;

TEST_CASE("Convert from string to InputSignal") {
  std::string const input{"12345678"};
  InputSignal signal = FFT::fromString(input, 1);

  InputSignal expected{1, 2, 3, 4, 5, 6, 7, 8};
  REQUIRE(expected == signal);
}

TEST_CASE("FFT simple input") {
  InputSignal input{1, 2, 3, 4, 5, 6, 7, 8};

  OutputSignal phase_1 = FFT::outputSignal(input);
  OutputSignal expected_1{4, 8, 2, 2, 6, 1, 5, 8};

  REQUIRE(phase_1 == expected_1);

  OutputSignal phase_2 = FFT::outputSignal(phase_1);
  OutputSignal expected_2{3, 4, 0, 4, 0, 4, 3, 8};

  REQUIRE(phase_2 == expected_2);

  OutputSignal phase_3 = FFT::outputSignal(phase_2);
  OutputSignal expected_3{0,3,4,1,5,5,1,8};

  REQUIRE(phase_3 == expected_3);

  OutputSignal phase_4 = FFT::outputSignal(phase_3);
  OutputSignal expected_4{0,1,0,2,9,4,9,8};

  REQUIRE(phase_4 == expected_4);
}

TEST_CASE("FFT longer input") {
  auto signal = FFT::fromString("80871224585914546619083218645595", 1);
  for (int i = 0; i < 100; ++i)
    signal = FFT::outputSignal(signal);;

  auto result =
      OutputSignal{std::begin(signal), std::next(std::begin(signal), 8)};
  auto expected = FFT::fromString("24176176", 1);

  REQUIRE(expected == result);
}

TEST_CASE("FFT longer input with many repetitions") {
  std::string const input{"03036732577212944063491565474664"};
  auto signal = FFT::fromString(input, 100);
  auto offset = std::stol(
      std::string{std::begin(input), std::next(std::begin(input), 7)});

  for (int i = 0; i < 100; ++i)
    signal = FFT::outputSignal(signal);

  auto result = OutputSignal{std::next(std::begin(signal), 0),
                             std::next(std::begin(signal), 0 + 8)};
  auto expected = FFT::fromString("84462026", 1);

  REQUIRE(expected == result);
}