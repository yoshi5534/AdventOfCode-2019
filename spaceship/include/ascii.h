#pragma once

#include <computer.h>
#include <droid.h>

#include <algorithm>
#include <iostream>
#include <map>
#include <numeric>
#include <tuple>
#include <vector>

namespace AdventOfCode {

using PixelCoordinate = MapPosition;
using Pixels = std::vector<PixelCoordinate>;
using Camera = std::map<PixelCoordinate, char>;

class ASCII {
public:
  ASCII(Program const &program);

  void print();

  bool isScaffold(PixelCoordinate coordinate);

  int findIntersections();

private:
  Computer computer_;
  Camera image_;
  int width_;
  int height_;
};

} // namespace AdventOfCode