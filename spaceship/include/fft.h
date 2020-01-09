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
  static Pattern get(int length, int element);
};

struct FFT {
  static InputSignal fromString(std::string const &text, int repetition);
  static OutputSignal outputSignal(InputSignal const &signal);
};

} // namespace AdventOfCode