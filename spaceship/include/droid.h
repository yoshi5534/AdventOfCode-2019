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
  constexpr bool operator==(MapPosition const &other) const {
    return ((x == other.x) && (y == other.y));
  }
  constexpr MapPosition operator+(MapPosition const &other) const {
    return {x + other.x, y + other.y};
  }
};

enum class Field { Unknown, Empty, Wall, Oxygen };

using DroidMap = std::map<MapPosition, Field>;

class Droid {
public:
  Droid(Program const &program);
  int shortestPath();
  int fillWithOxygen ();

private:
  bool exploreMap(int direction);
  bool move(int direction);

  Computer computer_;
  DroidMap area_;
  MapPosition droid_;
  MapPosition oxygen_;
};
} // namespace AdventOfCode