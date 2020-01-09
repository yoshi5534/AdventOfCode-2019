#pragma once

#include <algorithm>
#include <functional>
#include <numeric>
#include <string>
#include <vector>

namespace AdventOfCode {
using InputSignal = std::vector<int>;
using OutputSignal = std::vector<int>;
using Pattern = std::vector<int>;

struct PatternGenerator {
  static Pattern const BASE_PATTERN;

  static Pattern get(int length, int element) {
    Pattern pattern;
    pattern.resize(length + 1);
    auto it = std::begin(pattern);
    int index = 0;

    int number = element + 1;
    while (it < std::end(pattern)) {
      if (std::distance(it, std::end(pattern)) < number)
        number = std::distance(it, std::end(pattern));
      std::fill_n(it, number, BASE_PATTERN[index]);
      index = (index + 1) % BASE_PATTERN.size();
      std::advance(it, number);
    }

    return {std::begin(pattern) + 1, std::end(pattern)};
  }
};

struct FFT {
  static InputSignal fromString(std::string const &text) {
    InputSignal signal;
    signal.resize (text.size ());
    std::transform (std::begin (text), std::end (text), std::begin(signal), [](auto const character) {
      return character - '0';
    });

    return signal;
  }

  static int output(InputSignal const &signal, int element) {
    Pattern pattern = PatternGenerator::get(signal.size(), element);
    OutputSignal multiplied;
    multiplied.resize(signal.size());

    std::transform(std::begin(signal), std::end(signal), std::begin(pattern),
                   std::begin(multiplied), std::multiplies<int>());

    auto sum = std::accumulate(std::begin(multiplied), std::end(multiplied), 0);
    return std::abs(sum) % 10;
  }

  static OutputSignal outputSignal(InputSignal const &signal) {
    OutputSignal result;
    result.resize(signal.size());
    int index = 0;
    std::transform(std::begin(signal), std::end(signal), std::begin(result),
                   [&](auto const &) {
                     int element = output(signal, index);
                     index++;
                     return element;
                   });

    return result;
  }
};

} // namespace AdventOfCode