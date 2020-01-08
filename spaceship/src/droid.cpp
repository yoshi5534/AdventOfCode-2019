#include <droid.h>

#include <algorithm>
#include <iostream>
#include <map>

#include <spaceimage.h>

using namespace AdventOfCode;

Droid::Droid(Program const &program) : computer_{program}, droid_{0,0} {}

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

void printMap(DroidMap env) {
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
    std::cout << '\n';
  }
}

} // namespace

bool Droid::exploreMap(MapPosition currentPosition, int direction) {
  int startDirection = direction;
  printMap(area_);
  // make move
  computer_.writeInput({direction});
  Intcode lastCode = Intcode::Input;
  do {
    lastCode = computer_.runInstruction();
  } while (lastCode != Intcode::Output && lastCode != Intcode::Halt);
  if (lastCode == Intcode::Halt)
    return true;

  auto status = computer_.readOutput();
  if (status == 0) {
    area_[currentPosition] = Field::Wall;
    currentPosition = updatePosition(currentPosition, reverse (direction)); // we didn't move
  } else if (status == 1) {
    area_[currentPosition] = Field::Empty;
  } else if (status == 2) {
    area_[currentPosition] = Field::Oxygen;
    return true;
  }

  auto nextPosition = updatePosition(currentPosition, direction);
  if ((area_[nextPosition] == Field::Unknown) &&
      exploreMap(nextPosition, direction))
    return true;

  direction = turnRight(direction);
  nextPosition = updatePosition(currentPosition, direction);
  if ((area_[nextPosition] == Field::Unknown) &&
      exploreMap(nextPosition, direction))
    return true;

  direction = turnRight(direction);
  nextPosition = updatePosition(currentPosition, direction);
  if ((area_[nextPosition] == Field::Unknown) &&
      exploreMap(nextPosition, direction))
    return true;

  direction = turnRight(direction);
  nextPosition = updatePosition(currentPosition, direction);
  if ((area_[nextPosition] == Field::Unknown) &&
      exploreMap(nextPosition, direction))
    return true;

  // backtrack
  nextPosition = updatePosition(currentPosition, reverse(startDirection));
  exploreMap(nextPosition, reverse(startDirection));
  return false;
}

void Droid::move(int direction)
{
  
}