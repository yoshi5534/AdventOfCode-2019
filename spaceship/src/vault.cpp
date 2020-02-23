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
        vault_[{width, height}] = Field::Wall;
      if (c == '.')
        vault_[{width, height}] = Field::Open;
      if (c == '@')
        vault_[{width, height}] = Field::Entrance;
      if (c >= 'a' && c <= 'z')
        vault_[{width, height}] = Field::Key;
      if (c >= 'A' && c <= 'Z')
        vault_[{width, height}] = Field::Door;

      width++;
    }
    height++;
  }
}

int Vault::collectKeys() { return vault_.size(); }