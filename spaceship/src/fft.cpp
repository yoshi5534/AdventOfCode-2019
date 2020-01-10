#include <fft.h>

#include <algorithm>
#include <functional>
#include <numeric>
#include <string>
#include <vector>

using namespace AdventOfCode;

namespace {
static int output(InputSignal const &signal, int element) {
  Pattern const pattern = PatternGenerator::get(signal.size(), element);

  auto itPattern = std::begin(pattern);
  auto const sum =
      std::inner_product(std::begin(signal), std::end(signal), std::begin (pattern), 0);

  return std::abs(sum) % 10;
}
} // namespace

Pattern const PatternGenerator::BASE_PATTERN = {0, 1, 0, -1};

Pattern PatternGenerator::get(int length, int element) {
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
  int index = 0;
  std::transform(std::begin(signal), std::end(signal), std::begin(result),
                 [&](auto const &) { return output(signal, index++); });

  return result;
}
