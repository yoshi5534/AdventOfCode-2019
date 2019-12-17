#pragma once
#include <map>
#include <tuple>

#include <computer.h>
#include <spaceimage.h>

namespace AdventOfCode {

enum class Color { Black, White };

using Position = std::pair<int, int>;
using Grid = std::map<Position, Color>;

class EmergencyHullPaintingRobot {
public:
  EmergencyHullPaintingRobot(Program const &program);

  void paint(Color const &startColor);
  SpaceImage image() const;

private:
  Computer computer_;
  Grid surface_;
  int size_;
};
} // namespace AdventOfCode
