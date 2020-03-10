#include <tractorbeam.h>

#include <algorithm>

using namespace AdventOfCode;

int TractorBeam::scan(Program const &program) {
  int affected = 0;
  for (int x = 0; x < 50; ++x) {
    for (int y = 0; y < 50; ++y) {
      Computer computer{program};
      computer.writeInput({x, y});
      computer.calculate();
      affected += computer.readOutput();
    }
  }

  return affected;
}

namespace {
auto getLines(Program const &program) {
  std::vector<std::pair<int, int>> lines;
  lines.resize(1000);
  int first = 0;
  int last = 100;
  int y = lines.size();
  while (last - first < 200) {
    std::vector<int> line;
    for (int x = first; x < last + 10; ++x) {
      Computer computer{program};
      computer.writeInput({x, y});
      computer.calculate();
      line.push_back(computer.readOutput());
    }

    last = first + line.size() -
           std::distance(std::rbegin(line),
                         std::find(std::rbegin(line), std::rend(line), 1));
    first =
        first + std::distance(std::begin(line),
                              std::find(std::begin(line), std::end(line), 1));
    lines.push_back({first, last});
    y++;
  }

  return lines;
}
} // namespace

std::pair<int, int> TractorBeam::minimumDistance(Program const &program) {
  auto lines = getLines(program);
  for (int row = 0; row < static_cast<int>(lines.size() - 100); ++row) {
    auto &&line = lines[row];
    if (line.second - line.first < 100)
      continue;

    if (line.second - 100 == lines[row + 99].first)
      return {line.second - 100, row};
  }
  return {0, 0};
}