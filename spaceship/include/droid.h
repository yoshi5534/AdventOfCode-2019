#pragma once

#include <computer.h>
#include <map.h>

#include <map>

namespace AdventOfCode {

enum class Field { Unknown, Empty, Wall, Oxygen };

using DroidMap = std::map<MapPosition, Field>;

class Droid {
public:
  Droid(Program const &program);
  int shortestPath();
  int fillWithOxygen ();

private:
  bool exploreMap(int direction);
  bool move(int direction);

  Computer computer_;
  DroidMap area_;
  MapPosition droid_;
  MapPosition oxygen_;
};
} // namespace AdventOfCode