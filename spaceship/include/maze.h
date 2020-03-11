#pragma once

#include <map.h>

#include <iosfwd>

namespace AdventOfCode {
class Maze {
public:
  Maze(std::istream &map);

  int shortestPath();
private:
  Map maze_;
};
} // namespace AdventOfCode