#include <fft.h>

#include <algorithm>
#include <chrono>
#include <functional>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

using namespace AdventOfCode;

namespace {
static int output(InputSignal const &signal, int element) {
  int sum = 0;
  auto const period = (element + 1) * 2;

  auto const itSignal = std::begin(signal);
  auto const items = signal.size();

  auto s1 = std::chrono::high_resolution_clock::now();
  if ((period + element) < items) {
    int i = 0;
    int factor = 1;
    for (i = element; i < (items - period); i += period) {
      sum += std::accumulate(std::next(itSignal, i),
                             std::next(itSignal, i + element + 1), 0) *
             factor;
      factor *= -1;
    }

    if (i < items) {
      if (i + element < items) {
        sum += std::accumulate(std::next(itSignal, i),
                               std::next(itSignal, i + element + 1), 0) *
               factor;
      } else {
        sum += std::accumulate(std::next(itSignal, i), std::end(signal), 0) *
               factor;
      }
    }
  } else {
    if ((element + element) < items) {
      sum = std::accumulate(std::next(itSignal, element),
                            std::next(itSignal, element + element + 1), 0);
    } else {
      sum = std::accumulate(std::next(itSignal, element), std::end(signal), 0);
    }
  }

  auto s2 = std::chrono::high_resolution_clock::now();
  auto sd =
      std::chrono::duration_cast<std::chrono::microseconds>(s2 - s1).count();
 // std::cout << "Sum: " << sd << "ms \n";

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
