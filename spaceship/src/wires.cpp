#include "wires.h"
#include <algorithm>
#include <iostream>
#include <sstream>
#include <variant>

using namespace AdventOfCode;

namespace {
struct Direction {
  Direction(int l) : length{l} {}
  int length;
};

struct Right : public Direction {};

struct Left : public Direction {};

struct Up : public Direction {};

struct Down : public Direction {};

struct Tracer {
  WirePath &path_;

  Tracer(WirePath &path) : path_{path} {}

  void follow(int length, int xDir, int yDir) {
    auto location = path_.back();
    for (int i = 0; i < length; ++i)
      path_.push_back(Location{std::get<0>(location) + (i + 1) * xDir,
                               std::get<1>(location) + (i + 1) * yDir});
  }

  void operator()(Right const &direction) { follow(direction.length, 1, 0); }
  void operator()(Left const &direction) { follow(direction.length, -1, 0); }
  void operator()(Up const &direction) { follow(direction.length, 0, 1); }
  void operator()(Down const &direction) { follow(direction.length, 0, -1); }
};

using Trace = std::variant<Right, Left, Up, Down>;

Trace getTrace(std::string const &trace) {
  auto const dir = trace[0];
  auto const length = std::stoi(trace.substr(1));

  if (dir == 'R') {
    return Right{length};
  } else if (dir == 'L') {
    return Left{length};
  } else if (dir == 'U') {
    return Up{length};
  } else if (dir == 'D') {
    return Down{length};
  }

  return Right{0};
}

WirePath &operator<<(WirePath &path, Trace const &trace) {
  std::visit(Tracer{path}, trace);
  return path;
}
} // namespace

WirePath AdventOfCode::fromString(std::string const &wire) {
  WirePath path{};
  path.push_back({0, 0});
  std::istringstream wirestream{wire};
  std::string token;

  while (std::getline(wirestream, token, ',')) {
    path << getTrace(token);
  }

  return path;
}

Intersections AdventOfCode::intersects(WirePath path1, WirePath path2) {
  std::sort(std::begin(path1), std::end(path1));
  std::sort(std::begin(path2), std::end(path2));

  Intersections crossings(path1.size() + path2.size());
  auto it = std::set_intersection(std::begin(path1), std::end(path1),
                                  std::begin(path2), std::end(path2),
                                  crossings.begin());
  crossings.resize(it - crossings.begin());

  return crossings;
}

ManhattenDistances AdventOfCode::distances(Intersections const &crossings) {
  ManhattenDistances result{};
  for (auto const &location : crossings) {
    int distance =
        std::abs(std::get<0>(location)) + std::abs(std::get<1>(location));
    if (distance > 0)
      result.push_back(distance);
  }
  std::sort(std::begin(result), std::end(result));
  return result;
}

int AdventOfCode::minimalSignalDelay(WirePath const &path1,
                                     WirePath const &path2,
                                     Intersections const &crossings) {
  std::vector<int> steps{};
  for (auto const location : crossings) {
    auto steps1 =
        std::find(std::begin(path1), std::end(path1), location) - path1.begin();
    auto steps2 =
        std::find(std::begin(path2), std::end(path2), location) - path2.begin();

    auto total = steps1 + steps2;
    if (total > 0)
      steps.push_back(total);
  }

  std::sort(std::begin(steps), std::end(steps));
  return steps[0];
}