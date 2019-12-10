#include <orbit.h>

#include <range/v3/view.hpp>
#include <tuple>

using namespace AdventOfCode;

void Map::addObject(std::string const &name) {
  auto it = orbits_.find(name);
  if (it == orbits_.end()) {
    orbits_[name] = new Object{.name = name};
  }
}

std::tuple<std::string, std::string> splitEntry(std::string const &entry) {
  using namespace ranges;
  auto splitEntry =
      entry | views::split(')') | ranges::to<std::vector<std::string>>();
  return {splitEntry[0], splitEntry[1]};
}

void Map::addOrbit(std::string const &mapEntry) {
  if (mapEntry.empty())
    return;

  auto [center, orbiter] = splitEntry(mapEntry);

  addObject(center);
  addObject(orbiter);

  Object *c = (orbits_.find(center)->second);
  Object *o = (orbits_.find(orbiter)->second);
  c->orbiters.push_back(o);
}

int Map::checksum() const {
  int sum = 0;
  for (auto const obj : orbits_) {
    sum += findPath("COM", obj.first);
  }

  return sum;
}

int Map::minimumOrbitalTransfer(std::string const &start,
                                std::string const &target) const {

  int minimum = INT_MAX;

  for (auto const obj : orbits_) {
    int pathStart = findPath(obj.first, start);
    if (pathStart == 0)
      continue;
    int pathTarget = findPath(obj.first, target);
    if (pathTarget == 0)
      continue;

    int sum = pathStart - 1 + pathTarget - 1;
    if (sum < minimum)
      minimum = sum;
  }

  return minimum;
}

namespace {
int length(std::string const &name, std::vector<Object *> orbiters, int path) {
  for (auto const o : orbiters) {
    if (o->name == name)
      return path + 1;
    else {
      int p = length(name, o->orbiters, 0);
      if (p > 0)
        return p + 1;
    }
  }

  return 0;
}
} // namespace

int Map::findPath(std::string const &start, std::string const &name) const {
  int path = 0;
  if (start == name)
    return path;
  auto it = orbits_.find(name);
  if (it == orbits_.end())
    return path;

  for (auto const obj : orbits_) {
    int l = length(name, orbits_.find(start)->second->orbiters, 0);
    if (l > 0)
      return l;
  }

  return 0;
}