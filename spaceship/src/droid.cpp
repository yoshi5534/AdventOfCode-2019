#include <droid.h>

#include <algorithm>
#include <iostream>
#include <map>

#include <spaceimage.h>

using namespace AdventOfCode;

Droid::Droid(Program const &program) : computer_{program}, droid_{0, 0} {}

namespace {
MapPosition updatePosition(MapPosition const &current, int direction) {
  if (direction == 1)
    return {current.x, current.y - 1};
  if (direction == 2)
    return {current.x, current.y + 1};
  if (direction == 3)
    return {current.x - 1, current.y};
  if (direction == 4)
    return {current.x + 1, current.y};

  return current;
}

int turnRight(int direction) {
  if (direction == 1)
    return 4;
  if (direction == 2)
    return 3;
  if (direction == 3)
    return 1;
  if (direction == 4)
    return 2;

  return direction;
}

int turnLeft(int direction) {
  if (direction == 1)
    return 3;
  if (direction == 2)
    return 4;
  if (direction == 3)
    return 2;
  if (direction == 4)
    return 1;

  return direction;
}

int reverse(int direction) {
  if (direction == 1)
    return 2;
  if (direction == 2)
    return 1;
  if (direction == 3)
    return 4;
  if (direction == 4)
    return 3;

  return direction;
}

void printMap(DroidMap env, MapPosition const &droid) {
  int const width = 50;
  int const height = 50;
  Format data(width * height);

  std::for_each(std::begin(env), std::end(env), [&](auto const &field) {
    auto pos = field.first;
    auto x = (width / 2) + pos.x;
    auto y = (height / 2) + pos.y;
    int color = 0;
    switch (field.second) {
    case Field::Unknown:
      color = 0;
      break;
    case Field::Wall:
      color = 1;
      break;
    case Field::Empty:
      color = 2;
      break;
    case Field::Oxygen:
      color = 3;
      break;
    }

    data[y * width + x] = color;
  });

  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      if (x == ((width / 2) + droid.x) && y == ((height / 2) + droid.y)) {
        std::cout << 'D';
      } else {
        int color = data[y * width + x];
        if (color == 1)
          std::cout << '#';
        else if (color == 2)
          std::cout << '.';
        else if (color == 3)
          std::cout << 'O';
        else
          std::cout << ' ';
      }
    }
    std::cout << '\n';
  }
}

} // namespace

bool Droid::exploreMap(int direction) {
  printMap(area_, droid_);
  // make move
  if(!move(direction))
    return false;

  int startDirection = direction;

  if (area_[droid_] == Field::Oxygen)
    return true;

  auto nextPosition = updatePosition(droid_, 1);
  if ((area_[nextPosition] == Field::Unknown) &&
      exploreMap(1))
    return true;

  nextPosition = updatePosition(droid_, 3);
  if ((area_[nextPosition] == Field::Unknown) &&
      exploreMap(3))
    return true;

  nextPosition = updatePosition(droid_, 2);
  if ((area_[nextPosition] == Field::Unknown) &&
      exploreMap(2))
    return true;

  nextPosition = updatePosition(droid_, 4);
  if ((area_[nextPosition] == Field::Unknown) &&
      exploreMap(4))
    return true;

  // backtrack
  move(reverse(startDirection));
  printMap(area_, droid_);
  return false;
}

bool Droid::move(int direction) {
  droid_ = updatePosition(droid_, direction);

  computer_.writeInput({direction});
  Intcode lastCode = Intcode::Input;
  do {
    lastCode = computer_.runInstruction();
  } while (lastCode != Intcode::Output && lastCode != Intcode::Halt);
  if (lastCode == Intcode::Halt)
    return false;

  auto status = computer_.readOutput();
  if (status == 0) {
    area_[droid_] = Field::Wall;
    droid_ = updatePosition(droid_, reverse(direction)); // we didn't move
    return false;
  } else if (status == 1) {
    area_[droid_] = Field::Empty;
  } else if (status == 2) {
    area_[droid_] = Field::Oxygen;
  }

  return true;
}