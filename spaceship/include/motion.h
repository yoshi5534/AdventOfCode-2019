#pragma once

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <regex>
#include <string>
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

auto pos(Moon moon) { return std::get<0>(moon); }
auto vel(Moon moon) { return std::get<1>(moon); }
auto pot(Moon moon) {
  return std::abs(pos(moon).x) + std::abs(pos(moon).y) + std::abs(pos(moon).z);
}
auto kin(Moon moon) {
  return std::abs(vel(moon).x) + std::abs(vel(moon).y) + std::abs(vel(moon).z);
}
auto tot(Moon moon) { return pot(moon) * kin(moon); }

class Motion {
public:
  void addMoon(std::string const &moon) {
    std::regex positionPattern{"(-?\\d+)"};
    auto matches =
        std::sregex_iterator(std::begin(moon), std::end(moon), positionPattern);

    int x = std::stoi(matches->str());
    matches++;
    int y = std::stoi(matches->str());
    matches++;
    int z = std::stoi(matches->str());

    moons_.push_back({{x, y, z}, {0, 0, 0}});
  }

  int difference(int left, int right) const {
    if (left < right)
      return 1;
    else if (left > right)
      return -1;
    else
      return 0;
  }

  Moon applyGravity(Moon const &moon, Planet planet) {
    Velocity applied = vel(moon);
    Position const moon_pos = pos(moon);
    std::for_each(std::begin(planet), std::end(planet), [&](auto const &other) {
      Position const other_pos = pos(other);
      applied.x += difference(moon_pos.x, other_pos.x);
      applied.y += difference(moon_pos.y, other_pos.y);
      applied.z += difference(moon_pos.z, other_pos.z);
    });

    return {moon_pos, applied};
  }

  void timestep() {
    Planet afterTimeStep;
    std::for_each(std::begin(moons_), std::end(moons_), [&](auto const &moon) {
      auto gravitized = applyGravity(moon, moons_);
      auto velocity = vel(gravitized);
      auto moved = pos(gravitized) + velocity;
      afterTimeStep.push_back({moved, velocity});
    });

    moons_ = afterTimeStep;
  }

  int totalEnergy () const{
    int totalEnergy = 0;
    std::for_each(std::begin(moons_), std::end(moons_), [&](auto const &moon) {
      totalEnergy += tot(moon);
    });
    std::cout << "Sum of total energy: " << totalEnergy << "\n";
    return totalEnergy;
  }

  void print() const {
    std::for_each(std::begin(moons_), std::end(moons_), [](auto const &moon) {
      std::cout << "pos=<x=" << std::setw(3) << pos(moon).x
                << ", y=" << std::setw(3) << pos(moon).y
                << ", z=" << std::setw(3) << pos(moon).z
                << ">, vel=<x=" << std::setw(3) << vel(moon).x
                << ", y=" << std::setw(3) << vel(moon).y
                << ", z=" << std::setw(3) << vel(moon).z << ">\n";
    });
  }

private:
  Planet moons_;
};

} // namespace AdventOfCode
