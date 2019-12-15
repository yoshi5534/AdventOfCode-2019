#pragma once

#include <set>
#include <tuple>

namespace AdventOfCode {
using Coordinates = std::tuple<int, int>;
using AsteroidMap = std::set<Coordinates>;

Coordinates operator+(Coordinates const &left, Coordinates const &right); 

class Asteroids {
public:
  Asteroids(AsteroidMap asteroids, int width, int height);

  void print() const;
  int count() const;

  bool hasAsteroid(Coordinates coords) const;
  void remove(Coordinates coords);
  bool isInMap(Coordinates coords);

  AsteroidMap getLineCoords(Coordinates start, Coordinates offset);
  void removeLine(Coordinates start, Coordinates offset);

  int mostVisible () const;

private:
  AsteroidMap asteroids_;
  int width_;
  int height_;
};
} // namespace AdventOfCode
