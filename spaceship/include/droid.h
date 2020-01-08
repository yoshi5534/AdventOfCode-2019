#pragma once

#include <computer.h>

#include <map>

namespace AdventOfCode {
struct MapPosition {
  int x;
  int y;

  constexpr bool operator<(MapPosition const &other) const {
    if (y < other.y)
      return true;
    if (y == other.y)
      return x < other.x;

    return false;
  }
};

enum class Field { Unknown, Empty, Wall, Oxygen };

using DroidMap = std::map<MapPosition, Field>;

class Droid {
public:
  Droid(Program const &program);

  bool exploreMap(MapPosition currentPosition, int direction);
  void move (int direction);

private:
  Computer computer_;
  DroidMap area_;
  MapPosition droid_;
};
} // namespace AdventOfCode