#pragma once

#include <tuple>
#include <vector>

namespace AdventOfCode {
struct Vector {
  int64_t x;
  int64_t y;
  int64_t z;

  Vector operator+(Vector const &other) const {
    return {x + other.x, y + other.y, z + other.z};
  }
  Vector operator*(Vector const &other) const {
    return {x * other.x, y * other.y, z * other.z};
  }
  bool operator==(Vector const &other) const {
    return x == other.x && y == other.y && z == other.z;
  }
};

using Position = Vector;
using Velocity = Vector;
using Moon = std::tuple<Position, Velocity>;
using Planet = std::vector<Moon>;

class Motion {
public:
  void addMoon(std::string const &moon);
  void timestep();
  int totalEnergy() const;
  void print() const;
  int64_t repeatingTime ();

private:
  Planet moons_;
};

} // namespace AdventOfCode
