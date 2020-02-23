#include <vault.h>

#include <algorithm>
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
          std::holds_alternative<Key>(map.at(field)) ||
          std::holds_alternative<Entrance>(map.at(field)));
}

bool isKey(VaultMap const &map, MapPosition const &field) {
  return std::holds_alternative<Key>(map.at(field));
}

bool hasDoors(VaultMap const &map) {
  return std::find_if(std::begin(map), std::end(map), [](auto const &field) {
           return std::holds_alternative<Door>(field.second);
         }) != std::end(map);
}

int collectKey(VaultMap const &map, MapPosition const &start,
                      int steps) {

  std::vector<MapPosition> directions = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
  Node *current = nullptr;
  NodeSet openSet, closedSet;
  openSet.insert(new Node(start));
  while (!openSet.empty()) {
    current = *openSet.begin();
    for (auto node : openSet) {
      if (node->getScore() <= current->getScore()) {
        current = node;
      }
    }

    if (isKey(map, current->coordinates_))
      break;

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

  auto keyPosition = current->coordinates_;
  char key = std::get<Key>(map.at(keyPosition)).key;

  int pathLength = 0;
  while (current->parent_ != nullptr) {
    pathLength++;
    current = current->parent_;
  }

  if (!hasDoors(map))
    return steps + pathLength;


  auto doorPosition =
      std::find_if(std::begin(map), std::end(map),
                   [key](auto const &field) {
                     return std::holds_alternative<Door>(field.second) &&
                            std::get<Door>(field.second).door ==
                                std::toupper(key);
                   })
          ->first;


  releaseNodes(openSet);
  releaseNodes(closedSet);

  VaultMap newMap = map;
  newMap[keyPosition] = Open{};
  newMap[doorPosition] = Open{};

  int shortest = INT32_MAX;
  for (uint i = 0; i < directions.size(); ++i) {
    MapPosition newCoordinates(keyPosition + directions[i]);
    if (!isReachable(map, newCoordinates)) {
      continue;
    }

    auto length = collectKey(newMap, newCoordinates, steps + pathLength + 1);
    if (length < shortest)
      shortest = length;
  }

  return shortest;
}

} // namespace

int Vault::collectKeys() {
  auto entrance = findEntrance(vault_);
  VaultMap map = vault_;
  map[entrance] = Open{};

  return collectKey(map, entrance, 0);
}