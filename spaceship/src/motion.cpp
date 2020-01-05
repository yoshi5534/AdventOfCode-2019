#include <motion.h>

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <regex>
#include <string>

using namespace AdventOfCode;

namespace {

auto pos(Moon const &moon) { return std::get<0>(moon); }
auto vel(Moon const &moon) { return std::get<1>(moon); }
auto pot(Moon const &moon) {
  return std::abs(pos(moon).x) + std::abs(pos(moon).y) + std::abs(pos(moon).z);
}
auto kin(Moon const &moon) {
  return std::abs(vel(moon).x) + std::abs(vel(moon).y) + std::abs(vel(moon).z);
}
auto tot(Moon const &moon) { return pot(moon) * kin(moon); }

int difference(int left, int right) {
  if (left < right)
    return 1;
  else if (left > right)
    return -1;
  else
    return 0;
}

Moon applyGravity(Moon const &moon, Planet const &planet) {
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
} // namespace

void Motion::addMoon(std::string const &moon) {
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

void Motion::timestep() {
  Planet afterTimeStep;
  std::for_each(std::begin(moons_), std::end(moons_), [&](auto const &moon) {
    auto const gravitized = applyGravity(moon, moons_);
    auto const velocity = vel(gravitized);
    auto const moved = pos(gravitized) + velocity;
    afterTimeStep.push_back({moved, velocity});
  });

  moons_ = afterTimeStep;
}

int Motion::totalEnergy() const {
  int totalEnergy = 0;
  std::for_each(std::begin(moons_), std::end(moons_),
                [&](auto const &moon) { totalEnergy += tot(moon); });
  return totalEnergy;
}

void Motion::print() const {
  std::for_each(std::begin(moons_), std::end(moons_), [](auto const &moon) {
    std::cout << "pos=<x=" << std::setw(3) << pos(moon).x
              << ", y=" << std::setw(3) << pos(moon).y << ", z=" << std::setw(3)
              << pos(moon).z << ">, vel=<x=" << std::setw(3) << vel(moon).x
              << ", y=" << std::setw(3) << vel(moon).y << ", z=" << std::setw(3)
              << vel(moon).z << ">\n";
  });
}

int64_t Motion::repeatingTime() {
  auto starting = moons_;
  Vector counts{0, 0, 0};

  int64_t step = 0;
  while (counts.x == 0 || counts.y == 0 || counts.z == 0) {
    timestep();
    step++;

    bool xSame = true;
    bool ySame = true;
    bool zSame = true;
    for (int i = 0; i < 4; ++i) {
      if (pos(starting[i]).x != pos(moons_[i]).x ||
          vel(starting[i]).x != vel(moons_[i]).x)
        xSame = false;
      if (pos(starting[i]).y != pos(moons_[i]).y ||
          vel(starting[i]).y != vel(moons_[i]).y)
        ySame = false;
      if (pos(starting[i]).z != pos(moons_[i]).z ||
          vel(starting[i]).z != vel(moons_[i]).z)
        zSame = false;
    }

    if (xSame && counts.x == 0)
      counts.x = step;
    if (ySame && counts.y == 0)
      counts.y = step;
    if (zSame && counts.z == 0)
      counts.z = step;
  }

  std::vector<int64_t> steps{counts.x, counts.y, counts.z};
  std::sort(std::begin(steps), std::end(steps));
  auto result = std::accumulate(std::begin(steps), std::end(steps), static_cast <int64_t> (1), std::lcm<int64_t,int64_t>);
  return result;
}