#pragma once

#include <map.h>

#include <iosfwd>
#include <map>
#include <variant>

namespace AdventOfCode {

struct Wall{};
struct Open{};
struct Key{char key;};
struct Door{char door;};

using VaultField = std::variant<Wall, Open, Key, Door>;
using VaultMap = std::map<MapPosition, VaultField>;

class Vault {
public:
  Vault(std::istream &map);
  int collectKeys();
  void deployRobots();

private:
  VaultMap vault_;
  int width_;
  int height_;
};
} // namespace AdventOfCode