#pragma once

#include <computer.h>

namespace AdventOfCode {

struct TractorBeam {
  static int scan(Program const &program);
  static std::pair<int,int> minimumDistance(Program const &program);
};
} // namespace AdventOfCode