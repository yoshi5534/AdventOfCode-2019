#include <asteroids.h>

#include <algorithm>
#include <iostream>
#include <map>
using namespace AdventOfCode;

namespace {
int getX(Coordinates c) { return std::get<0>(c); }
int getY(Coordinates c) { return std::get<1>(c); }
} // namespace

namespace AdventOfCode {
Coordinates operator+(Coordinates const &left, Coordinates const &right) {
  return {getX(left) + getX(right), getY(left) + getY(right)};
}
} // namespace AdventOfCode

Asteroids::Asteroids(AsteroidMap asteroids, int width, int height)
    : asteroids_{asteroids}, width_{width}, height_{height} {}

void Asteroids::print() const {
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

int Asteroids::count() const { return asteroids_.size(); }
bool Asteroids::hasAsteroid(Coordinates coords) const {
  return asteroids_.find(coords) != asteroids_.end();
}

void Asteroids::remove(Coordinates coords) {
  if (hasAsteroid(coords))
    asteroids_.erase(coords);
}

bool Asteroids::isInMap(Coordinates coords) {
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

AsteroidMap Asteroids::getLineCoords(Coordinates start, Coordinates offset) {
  if (offset == Coordinates{0, 0})
    return {};

  AsteroidMap line;
  Coordinates current = start + offset;
  while (isInMap(current)) {
    line.insert(current);
    current = current + offset;
  }
  return line;
}

void Asteroids::removeLine(Coordinates start, Coordinates offset) {
  auto line = getLineCoords(start, offset);
  std::for_each(std::begin(line), std::end(line),
                [&](auto current) { remove(current); });
}

namespace {
bool isInSightAndRemoveAll(Asteroids &asteroids, Coordinates candidate,
                           Coordinates offset) {
  if (asteroids.isInMap(candidate + offset)) {
    auto line = asteroids.getLineCoords(candidate, offset);
    if (std::any_of(std::begin(line), std::end(line),
                    [&asteroids](auto coords) {
                      return asteroids.hasAsteroid(coords);
                    })) {
      asteroids.removeLine(candidate, offset);
      return true;
    }
  }

  return false;
}

using MostVisibleAsteroids = std::pair<Coordinates, int>;
struct CompareVisibles {
  bool operator()(MostVisibleAsteroids const &left,
                  MostVisibleAsteroids const &right) {
    return left.second < right.second;
  }
};
} // namespace

int Asteroids::mostVisible() const {
  std::map<Coordinates, int> visibles;

  std::for_each(std::begin(asteroids_), std::end(asteroids_),
                [&](auto candidate) {
                  visibles[candidate] = 0;
                  Asteroids asteroids{asteroids_, width_, height_};
                  for (int x = 0; x < width_; ++x) {
                    for (int y = 0; y < height_; ++y) {
                      if (Coordinates{x, y} == Coordinates{0, 0})
                        continue;
                      {
                        Coordinates offset{-1 * x, -1 * y};
                        if (isInSightAndRemoveAll(asteroids, candidate, offset))
                          visibles[candidate]++;
                      }
                      {
                        Coordinates offset{-1 * x, y};
                        if (isInSightAndRemoveAll(asteroids, candidate, offset))
                          visibles[candidate]++;
                      }
                      {
                        Coordinates offset{x, -1 * y};
                        if (isInSightAndRemoveAll(asteroids, candidate, offset))
                          visibles[candidate]++;
                      }
                      {
                        Coordinates offset{x, y};
                        if (isInSightAndRemoveAll(asteroids, candidate, offset))
                          visibles[candidate]++;
                      }
                    }
                  }
                });

  auto max = std::max_element(std::begin(visibles), std::end(visibles),
                              CompareVisibles());
  return max->second;
}