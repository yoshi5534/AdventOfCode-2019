#pragma once

#include <algorithm>
#include <iostream>
#include <set>
#include <tuple>

namespace AdventOfCode {
using Coordinates = std::tuple<int, int>;
using AsteroidMap = std::set<Coordinates>;

int getX(Coordinates c) { return std::get<0>(c); }
int getY(Coordinates c) { return std::get<1>(c); }

Coordinates operator+(Coordinates const &left, Coordinates const &right) {
  return {getX(left) + getX(right), getY(left) + getY(right)};
}

class Asteroids {
public:
  Asteroids(AsteroidMap asteroids, int width, int height)
      : asteroids_{asteroids}, width_{width}, height_{height} {};

  void print() const {
    for (int y = 0; y < height_; ++y) {
      for (int x = 0; x < width_; ++x) {
        if (hasAsteroid({x, y}))
          std::cout << '#';
        else
          std::cout << '.';
      }
      std::cout << '\n';
    }
  }

  int count() const { return asteroids_.size(); }
  bool hasAsteroid(Coordinates coords) const {
    return asteroids_.find(coords) != asteroids_.end();
  }

  void remove(Coordinates coords) {
    if (hasAsteroid(coords))
      asteroids_.erase(coords);
  }

  bool isInMap(Coordinates coords) {
    if (getX(coords) < 0)
      return false;
    if (getX(coords) >= width_)
      return false;
    if (getY(coords) < 0)
      return false;
    if (getY(coords) >= height_)
      return false;

    return true;
  }

  AsteroidMap getLineCoords(Coordinates start, Coordinates offset) {
    if (offset == Coordinates{0, 0})
      return {};

    AsteroidMap line;
    Coordinates current = start+offset;
    while (isInMap(current)) {
      line.insert(current);
      current = current + offset;
    }
    return line;
  }

  void removeLine(Coordinates start, Coordinates offset) {
    auto line = getLineCoords(start, offset);
    std::for_each(std::begin(line), std::end(line),
                  [&](auto current) { 
                    remove(current); });
  }

private:
  AsteroidMap asteroids_;
  int width_;
  int height_;
};
} // namespace AdventOfCode
