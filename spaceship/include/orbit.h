#pragma once
#include <map>
#include <string>
#include <vector>

namespace AdventOfCode {

struct Object {
  Object(std::string const& obj);
  std::string name;
  std::vector<Object *> orbiters;
};

class Orbit {
public:
  void addObject(std::string const &name);
  void addOrbit(std::string const &mapEntry);

  int checksum() const;
  int minimumOrbitalTransfer(std::string const &start,
                             std::string const &target) const;

private:
  int findPath(std::string const &start, std::string const &object) const;
  std::map<std::string, Object *> orbits_;
};

} // namespace AdventOfCode