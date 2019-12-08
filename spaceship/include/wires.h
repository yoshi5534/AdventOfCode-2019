#pragma once
#include <string.h>
#include <tuple>
#include <vector>

namespace AdventOfCode {
using Location = std::tuple<int, int>;
using Intersections = std::vector<Location>;
using WirePath = std::vector<Location>;
using ManhattenDistances = std::vector<int>;

WirePath fromString(std::string const &wire);

Intersections intersects(WirePath path1, WirePath path2);

ManhattenDistances distances(Intersections const &crossings);

int minimalSignalDelay(WirePath const &path1, WirePath const &path2,
                       Intersections const &crossings);
} // namespace AdventOfCode