#include <fft.h>

#include <algorithm>
#include <functional>
#include <numeric>
#include <string>
#include <vector>

using namespace AdventOfCode;

namespace {
static int output(InputSignal const &signal, int element) {
  int sum = 0;
  auto const period = (element + 1) * 2;
  int const count = signal.size();

  for (int i = element, factor = 1; i < count; i += period) {
    int end = i + element + 1;
    sum += factor * std::accumulate(std::next(std::begin(signal), i),
                                    (end > count)
                                        ? std::end(signal)
                                        : std::next(std::begin(signal), end),
                                    0);
    factor *= -1;
  }
  return std::abs(sum) % 10;
}
} // namespace

InputSignal FFT::fromString(std::string const &text, int repetition) {
  InputSignal signal;
  signal.resize(text.size() * repetition);
  for (int i = 0; i < repetition; ++i)
    std::transform(std::begin(text), std::end(text),
                   std::next(std::begin(signal), i * text.size()),
                   [](auto const &character) { return character - '0'; });

  return signal;
}

OutputSignal FFT::outputSignal(InputSignal const &signal) {
  OutputSignal result;
  result.resize(signal.size());
  std::transform(
      std::begin(signal), std::end(signal), std::begin(result),
      [&, index = 0](auto const &) mutable { return output(signal, index++); });

  return result;
}