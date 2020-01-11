#pragma once

#include <computer.h>

namespace AdventOfCode {

class ASCII {
public:
  ASCII(Program const &program);
  int findIntersections();

private:
  Program program_;
};

} // namespace AdventOfCode