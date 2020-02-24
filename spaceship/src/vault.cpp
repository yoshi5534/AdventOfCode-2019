#include <vault.h>

#include <algorithm>
#include <iostream>
#include <set>
#include <sstream>
#include <vector>

using namespace AdventOfCode;

Vault::Vault(std::istream &map) : vault_{} {
  std::string line;

  int height = 0;
  int width = 0;
  while (std::getline(map, line)) {
    width = 0;
    for (char c : line) {
      if (c == '#')
        vault_[{width, height}] = Wall{};
      if (c == '.')
        vault_[{width, height}] = Open{};
      if (c == '@')
        vault_[{width, height}] = Entrance{};
      if (c >= 'a' && c <= 'z')
        vault_[{width, height}] = Key{c};
      if (c >= 'A' && c <= 'Z')
        vault_[{width, height}] = Door{c};

      width++;
    }
    height++;
  }
}

namespace {
int const MAX_PATH = 100000;

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

Node *findNodeOnList(NodeSet const &nodes, MapPosition const &coordinates) {
  for (auto node : nodes) {
    if (node->coordinates_ == coordinates) {
      return node;
    }
  }
  return nullptr;
}

MapPosition findEntrance(VaultMap const &map) {
  return std::find_if(std::begin(map), std::end(map),
                      [](auto const &field) {
                        return std::holds_alternative<Entrance>(field.second);
                      })
      ->first;
}

bool isReachable(VaultMap const &map, MapPosition const &field) {
  return (std::holds_alternative<Open>(map.at(field)) ||
          std::holds_alternative<Key>(map.at(field)));
}

bool isKey(VaultMap const &map, MapPosition const &field) {
  return std::holds_alternative<Key>(map.at(field));
}

bool isEntrance(VaultMap const &map, MapPosition const &field) {
  return std::holds_alternative<Entrance>(map.at(field));
}

bool hasKeys(VaultMap const &map) {
  return std::find_if(std::begin(map), std::end(map), [](auto const &field) {
           return std::holds_alternative<Key>(field.second);
         }) != std::end(map);
}

void printMap(VaultMap const &map) {
  int const width = 30;
  int const height = 10;

  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      if (map.find({x, y}) != std::end(map)) {
        if (isKey(map, {x, y}))
          std::cout << 'k';
        else if (isEntrance(map, {x, y}))
          std::cout << '@';
        else if (isReachable(map, {x, y}))
          std::cout << '.';
        else
          std::cout << '#';
      }
    }
    std::cout << "\n";
  }
}

int collectKey(VaultMap const &map, MapPosition const &start,
               MapPosition const &dir, int steps) {

  if (!hasKeys(map))
    return steps;

  std::vector<MapPosition> directions = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
  Node *current = nullptr;
  NodeSet openSet, closedSet;
  openSet.insert(new Node(start + dir));
  std::map<MapPosition, int> keyPositions;
  while (!openSet.empty()) {
    current = *openSet.begin();
    for (auto node : openSet) {
      if (node->getScore() <= current->getScore()) {
        current = node;
      }
    }

    if (isKey(map, current->coordinates_)) {
      if (keyPositions.find(current->coordinates_) == std::end(keyPositions)) {
        int pathLength = 0;
        Node *path = current;
        while (path->parent_ != nullptr) {
          pathLength++;
          path = path->parent_;
        }
        keyPositions[current->coordinates_] = pathLength;
        printMap(map);
      }

      closedSet.insert(current);
      openSet.erase(std::find(openSet.begin(), openSet.end(), current));
      continue;
    } else if (current->getScore() > MAX_PATH) {
      break;
    }

    closedSet.insert(current);
    openSet.erase(std::find(openSet.begin(), openSet.end(), current));

    for (uint i = 0; i < directions.size(); ++i) {
      MapPosition newCoordinates(current->coordinates_ + directions[i]);
      if (!isReachable(map, newCoordinates)) {
        continue;
      }

      uint totalCost = current->g_ + 10;

      Node *successor = findNodeOnList(openSet, newCoordinates);
      if (successor == nullptr) {
        successor = new Node(newCoordinates, current);
        successor->g_ = totalCost;
        successor->h_ = 1;
        openSet.insert(successor);
      } else if (totalCost < successor->g_) {
        successor->parent_ = current;
        successor->g_ = totalCost;
      }
    }
  }

  releaseNodes(openSet);
  releaseNodes(closedSet);

  if (keyPositions.size() == 0)
    return MAX_PATH;

  int shortest = MAX_PATH;
  std::for_each(
      std::begin(keyPositions), std::end(keyPositions),
      [&](auto const &foundKeys) {
        char key = std::get<Key>(map.at(foundKeys.first)).key;

        auto door = std::find_if(
            std::begin(map), std::end(map), [key](auto const &field) {
              return std::holds_alternative<Door>(field.second) &&
                     std::get<Door>(field.second).door == std::toupper(key);
            });

        VaultMap newMap = map;
        newMap[start] = Open{};
        newMap[foundKeys.first] = Entrance{};
        if (door != std::end(map))
          newMap[door->first] = Open{};

        for (uint i = 0; i < directions.size(); ++i) {
          if (!isReachable(map, foundKeys.first + directions[i])) {
            continue;
          }

          auto length = collectKey(newMap, foundKeys.first, directions[i],
                                   steps + foundKeys.second + 1);
          if (length < shortest)
            shortest = length;
        }
      });

  return shortest;
}

} // namespace

int Vault::collectKeys() {
  auto entrance = findEntrance(vault_);
  VaultMap map = vault_;

  std::vector<MapPosition> directions = {{-1, 0}, {1, 0}, {0, 1}, {0, -1}};
  std::vector<int> steps;

  for (uint i = 0; i < directions.size(); ++i) {
    if (!isReachable(map, entrance + directions[i])) {
      continue;
    }
    steps.push_back(collectKey(map, entrance, directions[i], 1));
  }

  return *std::min_element(std::begin(steps), std::end(steps));
}