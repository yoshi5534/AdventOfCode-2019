#pragma once

#include <set>
#include <tuple>

namespace AdventOfCode {
using Coordinates = std::tuple<int, int>;
using AsteroidMap = std::set<Coordinates>;

class Asteroids {
public:
  Asteroids(AsteroidMap asteroids, int width, int height);

  void print() const;
  std::pair<int, Coordinates> mostVisible() const;
  Coordinates vaporized(Coordinates station, int shot) const;

private:
  bool hasAsteroid(Coordinates coords) const;

  AsteroidMap asteroids_;
  int width_;
  int height_;
};
} // namespace AdventOfCode