#pragma once

#include <iosfwd>

namespace AdventOfCode {

  class Vault{
    public:
    Vault (std::istream& map);

    int collectKeys();

  };
}