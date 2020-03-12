#pragma once

#include <map.h>

#include <iosfwd>
#include <map>

namespace AdventOfCode {
class Maze {
public:
  Maze(std::istream &map);

  int shortestPath();
  int recursivePath();
private:
  Map maze_;
  MapPosition start_;
  MapPosition end_;
  std::map <MapPosition, MapPosition> portals_;
};
} // namespace AdventOfCode