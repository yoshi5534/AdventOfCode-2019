#include <map.h>

#include <sstream>

using namespace AdventOfCode;

Map::Map(std::istream &map) : fields_{}, width_{}, height_{} {
  std::string line;

  int height = 0;
  int width = 0;
  int robot = 0;
  while (std::getline(map, line)) {
    width = 0;
    for (char c : line) {
      fields_.emplace_back(c);
      width++;
    }
    height++;
  }

  width_ = width;
  height_ = height;
}

char Map::at(int x, int y) const { return fields_[y * width_ + x]; }
void Map::set(int x, int y, char c) { fields_[y * width_ + x] = c; }