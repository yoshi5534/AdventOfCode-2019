#include <asteroids.h>

#include <algorithm>
#include <cmath>
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

constexpr double pi() { return std::atan(1) * 4; }
auto relativePosition(Coordinates station, Coordinates astroid) {
  Coordinates vector{getX(astroid) - getX(station),
                     getY(astroid) - getY(station)};

  return vector;
}

auto angle(Coordinates station, Coordinates astroid) {
  auto vector = relativePosition(station, astroid);
  auto tangens = std::atan2(getX(vector), -getY(vector));

  if (tangens < 0.)
    return 2. * pi() + tangens;

  return tangens;
}

auto radius(Coordinates station, Coordinates astroid) {
  auto vector = relativePosition(station, astroid);

  return std::sqrt(getX(vector) * getX(vector) + getY(vector) * getY(vector));
}

using Radius = double;
using Phi = double;
using PolarCoordinates =
    std::map<Phi, std::set<std::pair<Radius, Coordinates>>>;

PolarCoordinates mapFromAsteroid(AsteroidMap const &original,
                                 Coordinates const &asteroid) {
  PolarCoordinates map;
  std::for_each(std::begin(original), std::end(original), [&](auto candidate) {
    if (candidate == asteroid)
      return;
    auto phi = angle(asteroid, candidate);
    auto r = radius(asteroid, candidate);
    map[phi].insert({r, candidate});
  });

  return map;
}

} // namespace

std::pair<int, Coordinates> Asteroids::mostVisible() const {
  std::set<std::pair<int, Coordinates>> visibleAsteroids;

  std::for_each(std::begin(asteroids_), std::end(asteroids_),
                [&](auto candidate) {
                  auto localMap = mapFromAsteroid(asteroids_, candidate);
                  visibleAsteroids.insert({localMap.size(), candidate});
                });

  return *std::rbegin(visibleAsteroids);
}

Coordinates Asteroids::vaporized(Coordinates station, int shot) const {
  auto polarCoordinates = mapFromAsteroid(asteroids_, station);

  int removed = 0;
  auto current = std::begin(polarCoordinates);
  while (removed < shot - 1) {
    removed++;
    auto line = current->second;
    auto destroyed = line.erase(std::begin(line));
    if (destroyed == std::end(line))
      current = polarCoordinates.erase(current);
    else
      current++;

    if (current == std::end(polarCoordinates))
      current = std::begin(polarCoordinates);
  }

  return std::begin(current->second)->second;
}