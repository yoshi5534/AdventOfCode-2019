#pragma once

#include <iosfwd>
#include <map>
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
  constexpr bool operator!=(MapPosition const &other) const {
    return ((x != other.x) || (y != other.y));
  }
  constexpr MapPosition operator+(MapPosition const &other) const {
    return {x + other.x, y + other.y};
  }
  std::string operator()() const {
    return std::to_string(x) + "," + std::to_string(y);
  }
};

using Positions = std::map <MapPosition, char>;

class Map {
public:
  Map(std::istream &map);
  std::vector<char>& fields();
  int width() const;
  int height() const;
  char at(MapPosition const &pos) const;
  void set(MapPosition const &pos, char c);
  int shortestPath(MapPosition const &start, MapPosition const &end) const;
  Positions find (std::vector<char> const& items, MapPosition const &start) const;
  void print() const;
private:
  std::vector<char> fields_;
  int width_;
  int height_;
};
} // namespace AdventOfCode