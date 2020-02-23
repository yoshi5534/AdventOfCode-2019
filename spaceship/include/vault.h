#pragma once

#include <map.h>

#include <iosfwd>
#include <map>

namespace AdventOfCode {

enum class Field { Entrance, Open, Wall, Key, Door };

using VaultMap = std::map<MapPosition, Field>;

class Vault {
public:
  Vault(std::istream &map);

  int collectKeys();

private:
  VaultMap vault_;
};
} // namespace AdventOfCode