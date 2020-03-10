#pragma once

#include <map.h>

#include <iosfwd>

namespace AdventOfCode {

class Vault {
public:
  Vault(std::istream &map);
  int collectKeys() const;
  void deployRobots();

private:
  Map vault_;
};
} // namespace AdventOfCode