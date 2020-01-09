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
    
    std::generate(std::begin(pattern), std::end(pattern), [element, n = 0] () mutable { return patternValue (n++, element); });
    return std::move(pattern);
    // return std::move<Pattern> ({std::begin(pattern) + 1, std::end(pattern)});
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
    auto const sum =
        std::accumulate(std::begin(signal), std::end(signal), 0,
                        [element, index = 0]  (auto const &init, auto const &current) mutable {
                          return init + current * PatternGenerator::patternValue(index++, element);
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