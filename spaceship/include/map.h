#pragma once

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
}