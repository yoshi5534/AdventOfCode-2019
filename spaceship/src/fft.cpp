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
  int index = 0;
  auto o1 = std::chrono::high_resolution_clock::now();
  std::transform(std::begin(signal), std::end(signal), std::begin(result),
                 [&](auto const &) { return output(signal, index++); });
  auto o2 = std::chrono::high_resolution_clock::now();
  auto od =
      std::chrono::duration_cast<std::chrono::microseconds>(o2 - o1).count();
  std::cout << "Signal: " << od << "us \n";

  return result;
}
