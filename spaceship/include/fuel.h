#pragma once
#include <math.h>

namespace AdventOfCode {
int fuel_calculation(int mass) {
  return static_cast<int>(std::floor(static_cast<double>(mass) / 3.0) - 2.0);
}

int fuel_for_module(int mass) {
  int fuel = 0;
  while ((mass = fuel_calculation(mass)) > 0)
    fuel += mass;

  return fuel;
}
}; // namespace AdventOfCode