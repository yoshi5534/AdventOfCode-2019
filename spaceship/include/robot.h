#pragma once
#include <computer.h>
#include <spaceimage.h>

namespace AdventOfCode {

enum class Color { Black, White };

class EmergencyHullPaintingRobot {
public:
  EmergencyHullPaintingRobot(Program const &program);

  SpaceImage paint(Color const &startColor);

private:
  Computer computer_;
};
} // namespace AdventOfCode
