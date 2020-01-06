#pragma once
#include <math.h>
#include <string>
#include <tuple>
#include <vector>

namespace AdventOfCode {
int fuel_calculation(int mass);
int fuel_for_module(int mass);

struct Reaction {
  std::vector <std::tuple <int,std::string> > inputs;

  std::tuple <int, std::string> output;
};

using PossibleReactions = std::vector<Reaction>;

class NanoFactory {
public:
  void addReaction(std::string const &reaction);
  int necessaryORE() const;

private:
  PossibleReactions reactions_;
};
} // namespace AdventOfCode