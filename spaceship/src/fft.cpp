#include <fft.h>

#include <algorithm>
#include <functional>
#include <numeric>
#include <string>
#include <vector>

using namespace AdventOfCode;

namespace {
static int output(Signal const &signal, int element) {
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

Signal FFT::fromString(std::string const &text, int repetition) {
  Signal signal;
  signal.resize(text.size() * repetition);
  for (int i = 0; i < repetition; ++i)
    std::transform(std::begin(text), std::end(text),
                   std::next(std::begin(signal), i * text.size()),
                   [](auto const &character) { return character - '0'; });

  return signal;
}

Signal FFT::outputSignal(Signal const &signal) {
  Signal result;
  result.resize(signal.size());
  std::transform(
      std::begin(signal), std::end(signal), std::begin(result),
      [&, index = 0](auto const &) mutable { return output(signal, index++); });

  return result;
}

Signal FFT::outputMessage(Signal const &signal, int repetition, int offset) {
  Signal input{std::next(std::begin(signal), offset), std::end(signal)};
  for (int i = 0; i < repetition; ++i) {
    Signal result;
    result.resize(input.size());

    std::transform(std::rbegin(input), std::rend(input), std::rbegin(result),
                   [&, index = -1](auto const &value) mutable {
                     return value + *std::next(std::rbegin(result), index++);
                   });

    std::transform(std::begin(result), std::end(result), std::begin(input),
                   [](auto const &value) { return value % 10; });
  }

  return {std::begin(input), std::next(std::begin(input), 8)};
}