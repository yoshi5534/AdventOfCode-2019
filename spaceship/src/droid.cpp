#include <droid.h>

#include <algorithm>
#include <iostream>
#include <map>
#include <set>

#include <spaceimage.h>

using namespace AdventOfCode;

Droid::Droid(Program const &program)
    : computer_{program}, area_{}, droid_{0, 0}, oxygen_{0, 0} {
  exploreMap(3);
}

namespace {
using CoordinateList = std::vector<MapPosition>;

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

void printMap(DroidMap const &env, MapPosition const &droid,
              MapPosition const &oxygen, CoordinateList const &path) {
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
      } else if (x == ((width / 2) + oxygen.x) &&
                 y == ((height / 2) + oxygen.y)) {
        std::cout << 'O';
      } else if (std::find(std::begin(path), std::end(path),
                           MapPosition{x - (width / 2), y - (height / 2)}) !=
                 std::end(path)) {
        std::cout << 'X';
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

struct Node {
  uint g_, h_;
  MapPosition coordinates_;
  Node *parent_;

  Node(MapPosition coord, Node *parent = nullptr)
      : g_{0}, h_{0}, coordinates_{coord}, parent_{parent} {}
  uint getScore() { return g_ + h_; }
};
using NodeSet = std::set<Node *>;

void releaseNodes(NodeSet &nodes) {
  for (auto it = nodes.begin(); it != nodes.end();) {
    delete *it;
    it = nodes.erase(it);
  }
}

Node *findNodeOnList(NodeSet &nodes, MapPosition coordinates) {
  for (auto node : nodes) {
    if (node->coordinates_ == coordinates) {
      return node;
    }
  }
  return nullptr;
}
bool detectCollision(DroidMap &area, MapPosition const &coordinates) {
  if ((area[coordinates] == Field::Wall) ||
      area[coordinates] == Field::Unknown) {
    return true;
  }
  return false;
}

MapPosition getDelta(MapPosition const &source, MapPosition const &target) {
  return {abs(source.x - target.x), abs(source.y - target.y)};
}

uint manhattan(MapPosition source, MapPosition target) {
  auto delta = std::move(getDelta(source, target));
  return static_cast<uint>(10 * (delta.x + delta.y));
}

bool hasEmptyFields(DroidMap const &area) {
  return std::find_if(std::begin(area), std::end(area), [](auto const &field) {
           return field.second == Field::Empty;
         }) != std::end(area);
}

bool isEmpty(DroidMap const &area, MapPosition coord) {
  return area.at(coord) == Field::Empty;
}
} // namespace

bool Droid::exploreMap(int direction) {
  if (!move(direction))
    return false;

  if (area_[droid_] == Field::Oxygen) {
    oxygen_ = droid_;
  }

  if (droid_ == MapPosition{0, 0})
    return true;

  auto nextPosition = updatePosition(droid_, 1);
  if ((area_[nextPosition] == Field::Unknown) && exploreMap(1))
    return true;

  nextPosition = updatePosition(droid_, 2);
  if ((area_[nextPosition] == Field::Unknown) && exploreMap(2))
    return true;

  nextPosition = updatePosition(droid_, 3);
  if ((area_[nextPosition] == Field::Unknown) && exploreMap(3))
    return true;

  nextPosition = updatePosition(droid_, 4);
  if ((area_[nextPosition] == Field::Unknown) && exploreMap(4))
    return true;

  // backtrack
  move(reverse(direction));
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

int Droid::shortestPath() {
  std::vector<MapPosition> directions = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};

  Node *current = nullptr;
  NodeSet openSet, closedSet;
  openSet.insert(new Node({0, 0}));
  while (!openSet.empty()) {
    current = *openSet.begin();
    for (auto node : openSet) {
      if (node->getScore() <= current->getScore()) {
        current = node;
      }
    }

    if (current->coordinates_ == oxygen_) {
      break;
    }

    closedSet.insert(current);
    openSet.erase(std::find(openSet.begin(), openSet.end(), current));

    for (uint i = 0; i < directions.size(); ++i) {
      MapPosition newCoordinates(current->coordinates_ + directions[i]);
      if (detectCollision(area_, newCoordinates) ||
          findNodeOnList(closedSet, newCoordinates)) {
        continue;
      }

      uint totalCost = current->g_ + 10;

      Node *successor = findNodeOnList(openSet, newCoordinates);
      if (successor == nullptr) {
        successor = new Node(newCoordinates, current);
        successor->g_ = totalCost;
        successor->h_ = manhattan(successor->coordinates_, oxygen_);
        openSet.insert(successor);
      } else if (totalCost < successor->g_) {
        successor->parent_ = current;
        successor->g_ = totalCost;
      }
    }
  }

  CoordinateList path;
  while (current != nullptr) {
    path.push_back(current->coordinates_);
    current = current->parent_;
  }

  releaseNodes(openSet);
  releaseNodes(closedSet);

  printMap(area_, droid_, oxygen_, path);
  return path.size() - 1;
}

int Droid::fillWithOxygen() {
  std::vector<MapPosition> directions = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};

  CoordinateList filled;
  filled.push_back(oxygen_);

  int time = 0;
  while (hasEmptyFields(area_)) {
    time++;
    std::for_each(std::begin(filled), std::end(filled), [&](auto const coord) {
      for (uint i = 0; i < directions.size(); ++i) {
        MapPosition neighbor(coord + directions[i]);
        if (!isEmpty(area_, neighbor))
          continue;

        filled.push_back(neighbor);
        area_[neighbor] = Field::Oxygen;
      }
    });
  }

  printMap(area_, droid_, oxygen_, {});
  return time;
}