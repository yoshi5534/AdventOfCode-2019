#pragma once

#include <iosfwd>
#include <string>
#include <vector>

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
  std::string operator()() const {
    return std::to_string(x) + "," + std::to_string(y);
  }
};

class Map {
public:
  Map(std::istream &map);
  char at(int x, int y) const;
  void set(int x, int y, char c);

private:
  std::vector<char> fields_;
  int width_;
  int height_;
};
} // namespace AdventOfCode