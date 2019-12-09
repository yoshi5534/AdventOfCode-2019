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

int Map::checksum() { return 0; }

int findPath(std::string const &name) const {
  int path = 0;
  if (object == "COM")
    return path;
  auto it = orbits_.find(object);
  if (it == orbits_.end())
    return path;
  
}