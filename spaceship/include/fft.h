#pragma once

#include <string>
#include <vector>

namespace AdventOfCode {
using Signal = std::vector<int>;

struct FFT {
  static Signal fromString(std::string const &text, int repetition);
  static Signal outputSignal(Signal const &signal);
  static Signal outputMessage(Signal const &signal, int repetition, int offset);
};

} // namespace AdventOfCode