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
    pattern.resize(length);

    auto it = std::begin(pattern);
    int index = 0;

    int number = element; // start with one less
    while (it < std::end(pattern)) {
      std::fill_n(it, number, BASE_PATTERN[index]);
      index = (index + 1) % BASE_PATTERN.size();
      std::advance(it, number);

      if (std::distance(it, std::end(pattern)) < number)
        number = std::distance(it, std::end(pattern));
      else
        number = element + 1;
    }

    return std::move(pattern);
  }

  static int patternValue(int index, int element) {
    index++;
    index %= (element + 1) * 4;
    index /= (element + 1);
    return BASE_PATTERN[index];
  }
};

struct FFT {
  static InputSignal fromString(std::string const &text, int repetition) {
    InputSignal signal;
    signal.resize(text.size() * repetition);
    for (int i = 0; i < repetition; ++i)
      std::transform(std::begin(text), std::end(text),
                     std::next(std::begin(signal), i * text.size()),
                     [](auto const &character) { return character - '0'; });

    return signal;
  }

  static int output(InputSignal const &signal, int element) {
    Pattern const pattern = PatternGenerator::get(signal.size(), element);

    auto itPattern = std::begin(pattern);
    auto const sum =
        std::accumulate(std::begin(signal), std::end(signal), 0,
                        [&itPattern](auto const &init, auto const &current) {
                          return init + current * *itPattern++;
                        });

    return std::abs(sum) % 10;
  }

  static OutputSignal outputSignal(InputSignal const &signal) {
    OutputSignal result;
    result.resize(signal.size());
    int index = 0;
    std::transform(std::begin(signal), std::end(signal), std::begin(result),
                   [&](auto const &) { return output(signal, index++); });

    return result;
  }
};

} // namespace AdventOfCode