#pragma once

#include <string>
#include <vector>

namespace AdventOfCode {
using InputSignal = std::vector<int>;
using OutputSignal = std::vector<int>;

struct FFT {
  static InputSignal fromString(std::string const &text, int repetition);
  static OutputSignal outputSignal(InputSignal const &signal);
};

} // namespace AdventOfCode