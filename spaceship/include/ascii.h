#pragma once

#include <computer.h>

namespace AdventOfCode {

class ASCII {
public:
  ASCII(Program const &program);
  int findIntersections();
  void searchRobots();
  void possibleMovements();

private:
  Program program_;
};

} // namespace AdventOfCode