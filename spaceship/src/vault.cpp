#include <vault.h>

#include <sstream>

using namespace AdventOfCode;

Vault::Vault(std::istream &map) : vault_{} {
  std::string line;

  int height = 0;
  int width = 0;
  while (std::getline(map, line)) {
    width = 0;
    for (char c : line) {
      if (c == '#')
        vault_[{width, height}] = Wall{};
      if (c == '.')
        vault_[{width, height}] = Open{};
      if (c == '@')
        vault_[{width, height}] = Entrance{};
      if (c >= 'a' && c <= 'z')
        vault_[{width, height}] = Key{c};
      if (c >= 'A' && c <= 'Z')
        vault_[{width, height}] = Door{c};

      width++;
    }
    height++;
  }
}

int Vault::collectKeys() { return vault_.size(); }