#pragma once
#include <string>
#include <map>
#include <vector>

namespace AdventOfCode {


struct Object {
  std::string name;
  std::map <std::string, Object*> orbiters;
};

class Map {
public:
  void addObject (std::string const& name);
  void addOrbit(std::string const& mapEntry);

  int checksum ();

private:
int findPath (std::string const& object) const;
std::map <std::string, Object*> orbits_;
};

} // namespace AdventOfCode