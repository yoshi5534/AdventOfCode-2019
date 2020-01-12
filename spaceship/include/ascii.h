#pragma once

#include <computer.h>

namespace AdventOfCode {

class ASCII {
public:
  ASCII(Program const &program);
  int findIntersections() const;
  void shortestPath() const;
  int searchRobots() const;

private:
  Program program_;
};

} // namespace AdventOfCode