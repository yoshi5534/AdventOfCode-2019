#include <vault.h>

#include <algorithm>
#include <iostream>
#include <set>
#include <sstream>
#include <vector>

using namespace AdventOfCode;

namespace {
using Path = std::string;

int quadrant(MapPosition const &position, int width, int height) {
  if (position.x < width / 2)
    if (position.y < height / 2)
      return 0;
    else
      return 2;
  else if (position.y < height / 2)
    return 1;
  else
    return 3;
}

auto findEntrances(VaultMap const &map, int width, int height) {
  std::vector<std::pair<MapPosition, VaultField>> entrances;
  std::copy_if(std::begin(map), std::end(map), std::back_inserter(entrances),
               [](auto const &field) {
                 if (std::holds_alternative<Key>(field.second)) {
                   auto key = std::get<Key>(field.second).key;
                   return key >= '0' && key <= '9';
                 }

                 return false;
               });

  if (entrances.size() == 1)
    return std::vector{entrances.begin()->first};

  std::vector<MapPosition> positions(entrances.size());
  for (auto const &field : entrances)
    positions[quadrant(field.first, width, height)] = field.first;

  return positions;
}

auto allDoors(VaultMap const &map) {
  std::map<char, MapPosition> doors;
  std::for_each(std::begin(map), std::end(map), [&](auto const &field) {
    if (std::holds_alternative<Door>(field.second)) {
      doors[std::tolower(std::get<Door>(field.second).door)] = field.first;
    }
  });

  return doors;
}

bool isReachable(VaultMap const &map, MapPosition const &field) {
  return (std::holds_alternative<Open>(map.at(field)) ||
          std::holds_alternative<Key>(map.at(field)));
}

bool isKey(VaultMap const &map, MapPosition const &field) {
  return std::holds_alternative<Key>(map.at(field));
}

MapPosition left(MapPosition const &dir) {
  if (dir == MapPosition{0, 1})
    return {1, 0};
  if (dir == MapPosition{1, 0})
    return {0, -1};
  if (dir == MapPosition{0, -1})
    return {-1, 0};
  if (dir == MapPosition{-1, 0})
    return {0, 1};

  return dir;
}

MapPosition right(MapPosition const &dir) {
  if (dir == MapPosition{0, 1})
    return {-1, 0};
  if (dir == MapPosition{1, 0})
    return {0, 1};
  if (dir == MapPosition{0, -1})
    return {1, 0};
  if (dir == MapPosition{-1, 0})
    return {0, -1};

  return dir;
}

MapPosition reverse(MapPosition const &dir) {
  if (dir == MapPosition{0, 1})
    return {0, -1};
  if (dir == MapPosition{1, 0})
    return {-1, 0};
  if (dir == MapPosition{0, -1})
    return {0, 1};
  if (dir == MapPosition{-1, 0})
    return {1, 0};

  return dir;
}

void printMap(VaultMap const &map) {
  int const width = 30;
  int const height = 10;

  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      if (map.find({x, y}) != std::end(map)) {
        if (isKey(map, {x, y}))
          std::cout << 'k';
        else if (isReachable(map, {x, y}))
          std::cout << '.';
        else
          std::cout << '#';
      }
    }
    std::cout << "\n";
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

Node *findNodeOnList(NodeSet const &nodes, MapPosition const &coordinates) {
  for (auto node : nodes) {
    if (node->coordinates_ == coordinates) {
      return node;
    }
  }
  return nullptr;
}

MapPosition getDelta(MapPosition const &source, MapPosition const &target) {
  return {abs(source.x - target.x), abs(source.y - target.y)};
}

uint manhattan(MapPosition const &source, MapPosition const &target) {
  auto delta = getDelta(source, target);
  return static_cast<uint>(10 * (delta.x + delta.y));
}

int shortestPath(VaultMap const &map, MapPosition const &start,
                 MapPosition const &end) {
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

    if (current->coordinates_ == end) {
      break;
    }

    closedSet.insert(current);
    openSet.erase(std::find(openSet.begin(), openSet.end(), current));

    for (uint i = 0; i < directions.size(); ++i) {
      MapPosition newCoordinates(current->coordinates_ + directions[i]);
      if (!isReachable(map, newCoordinates) ||
          findNodeOnList(closedSet, newCoordinates)) {
        continue;
      }

      uint totalCost = current->g_ + 10;

      Node *successor = findNodeOnList(openSet, newCoordinates);
      if (successor == nullptr) {
        successor = new Node(newCoordinates, current);
        successor->g_ = totalCost;
        successor->h_ = manhattan(successor->coordinates_, end);
        openSet.insert(successor);
      } else if (totalCost < successor->g_) {
        successor->parent_ = current;
        successor->g_ = totalCost;
      }
    }
  }

  int path = 0;
  while (current != nullptr) {
    path++;
    current = current->parent_;
  }

  releaseNodes(openSet);
  releaseNodes(closedSet);

  return path - 1;
}

bool nextKeys(std::map<MapPosition, char> &keys, VaultMap const &map,
              std::vector<MapPosition> visited, MapPosition const &current,
              MapPosition const &direction, int count) {
  if (std::find(std::begin(visited), std::end(visited), current) !=
      std::end(visited))
    return false;

  visited.push_back(current);

  if (!isKey(map, current)) {
    count++;
    if (isReachable(map, current + right(direction)))
      nextKeys(keys, map, visited, current + right(direction), right(direction),
               count);

    if (isReachable(map, current + left(direction)))
      nextKeys(keys, map, visited, current + left(direction), left(direction),
               count);

    if (isReachable(map, current + direction))
      nextKeys(keys, map, visited, current + direction, direction, count);

    if (isReachable(map, current + reverse(direction)))
      nextKeys(keys, map, visited, current + reverse(direction),
               reverse(direction), count);
  } else {
    keys[current] = std::get<Key>(map.at(current)).key;
    return true;
  }

  return false;
}

int allfrom(std::map<Path, int> &steps,
            std::map<Path, std::map<MapPosition, char>> &keyPaths, VaultMap map,
            int width, int height, std::vector<MapPosition> robots,
            std::map<Path, int> &allKeyDistances,
            std::map<char, MapPosition> const &doors, MapPosition const &start,
            Path path, int count) {

  auto key = std::get<Key>(map.at(start)).key;
  int startQuadrant = 0;
  if (robots.size() == 4)
    startQuadrant = quadrant(start, width, height);

  robots[startQuadrant] = start;

  for (auto const &robot : robots)
    map[robot] = Open{};

  if (doors.count(key))
    map[doors.at(key)] = Open{};

  auto abstractPath = path;
  std::sort(std::begin(abstractPath), std::end(abstractPath));
  abstractPath.push_back(key);

  std::map<MapPosition, char> keys;
  if (keyPaths.count(abstractPath))
    keys = keyPaths.at(abstractPath);
  else {
    for (auto const &robot : robots)
      nextKeys(keys, map, {}, robot, {-1, 0}, 0);
    keyPaths[abstractPath] = keys;
  }

  path.push_back(key);

  if (keys.empty()) {
    return count;
  }

  for (auto const robot : robots)
    abstractPath += robot();

  if (!steps.count(abstractPath)) {
    int minimum = 1000000;
    std::for_each(std::begin(keys), std::end(keys), [&](auto const &nextKey) {
      int quad = 0;
      if (robots.size() == 4)
        quad = quadrant(nextKey.first, width, height);

      auto nextKeyPath = std::to_string(robots[quad].x) + "," +
                         std::to_string(robots[quad].y) + "," +
                         std::to_string(nextKey.first.x) + "," +
                         std::to_string(nextKey.first.y);
      auto backKeyPath = std::to_string(nextKey.first.x) + "," +
                         std::to_string(nextKey.first.y) + "," +
                         std::to_string(robots[quad].x) + "," +
                         std::to_string(robots[quad].y);
      int distance = 0;
      if (allKeyDistances.count(nextKeyPath))
        distance = allKeyDistances[nextKeyPath];
      else {
        distance = shortestPath(map, robots[quad], nextKey.first);
        allKeyDistances[nextKeyPath] = distance;
        allKeyDistances[backKeyPath] = distance;
      }

      auto const length =
          allfrom(steps, keyPaths, map, width, height, robots, allKeyDistances,
                  doors, nextKey.first, path, distance);

      if (length < minimum)
        minimum = length;
    });
    steps[abstractPath] = minimum;
  }

  return steps[abstractPath] + count;
}

} // namespace

Vault::Vault(std::istream &map) : vault_{} {
  std::string line;

  int height = 0;
  int width = 0;
  int robot = 0;
  while (std::getline(map, line)) {
    width = 0;
    for (char c : line) {
      if (c == '#')
        vault_[{width, height}] = Wall{};
      if (c == '.')
        vault_[{width, height}] = Open{};
      if (c == '@')
        vault_[{width, height}] = Key{static_cast<char>('0' + robot++)};
      if (c >= 'a' && c <= 'z')
        vault_[{width, height}] = Key{c};
      if (c >= 'A' && c <= 'Z')
        vault_[{width, height}] = Door{c};

      width++;
    }
    height++;
  }

  width_ = width;
  height_ = height;
}

int Vault::collectKeys() {
  VaultMap map = vault_;
  auto entrances = findEntrances(map, width_, height_);
  auto doors = allDoors(map);

  std::map<Path, int> steps;
  std::map<MapPosition, char> keys;
  std::map<Path, int> allKeyDistances;
  std::map<Path, std::map<MapPosition, char>> keyPaths;

  auto length = allfrom(steps, keyPaths, map, width_, height_, entrances,
                        allKeyDistances, doors, entrances[0], {}, 0);

  return length;
}

void Vault::deployRobots() {
  auto entrance = *findEntrances(vault_, width_, height_).begin();
  vault_[entrance + MapPosition{-1, -1}] = Key{static_cast<char>('0')};
  vault_[entrance + MapPosition{0, -1}] = Wall{};
  vault_[entrance + MapPosition{1, -1}] = Key{static_cast<char>('1')};
  vault_[entrance + MapPosition{-1, 0}] = Wall{};
  vault_[entrance + MapPosition{0, 0}] = Wall{};
  vault_[entrance + MapPosition{1, 0}] = Wall{};
  vault_[entrance + MapPosition{-1, 1}] = Key{static_cast<char>('2')};
  vault_[entrance + MapPosition{0, 1}] = Wall{};
  vault_[entrance + MapPosition{1, 1}] = Key{static_cast<char>('3')};
}