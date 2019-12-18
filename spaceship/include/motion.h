#pragma once

#include <tuple>
#include <vector>

namespace AdventOfCode {
struct Vector {
  int x;
  int y;
  int z;

  Vector operator+(Vector const &other) const {
    return {x + other.x, y + other.y, z + other.z};
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

private:
  Planet moons_;
};

} // namespace AdventOfCode
