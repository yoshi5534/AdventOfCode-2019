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
  auto [center, orbiter] = splitEntry(mapEntry);

  addObject(center);
  addObject(orbiter);

  Object *c = (orbits_.find(center)->second);
  Object *o = (orbits_.find(orbiter)->second);
  c->orbiters.push_back(o);
}

int Map::checksum() {
  int sum = 0;
  for (auto const obj : orbits_) {
    sum += findPath(obj.first);
  }

  return sum;
}

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

int Map::findPath(std::string const &name) const {
  int path = 0;
  if (name == "COM")
    return path;
  auto it = orbits_.find(name);
  if (it == orbits_.end())
    return path;

  for (auto const obj : orbits_) {
    int l = length(name, orbits_.find("COM")->second->orbiters, 0);
    if (l > 0)
      return l;
  }

  return 0;
}