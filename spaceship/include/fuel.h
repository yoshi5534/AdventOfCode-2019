#pragma once
#include <map>
#include <string>
#include <tuple>
#include <vector>

namespace AdventOfCode {
int fuel_calculation(int mass);
int fuel_for_module(int mass);

using Chemical = std::tuple <std::string, int64_t>;

struct Reaction {
  std::vector <Chemical> inputs;
  Chemical output;
};

using PossibleReactions = std::map<std::string, Reaction>;

class NanoFactory {
public:
  void addReaction(std::string const &reaction);
  int64_t necessaryORE() const;
  int64_t maximumFuel(int64_t ore) const;

private:
  PossibleReactions reactions_;
};
} // namespace AdventOfCode