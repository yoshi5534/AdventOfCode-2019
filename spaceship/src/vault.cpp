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
using Path = std::vector<char>;

MapPosition findEntrance(VaultMap const &map) {
  return std::find_if(std::begin(map), std::end(map),
                      [](auto const &field) {
                        return std::holds_alternative<Entrance>(field.second);
                      })
      ->first;
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

bool isEntrance(VaultMap const &map, MapPosition const &field) {
  return std::holds_alternative<Entrance>(map.at(field));
}

bool hasKeys(VaultMap const &map) {
  return std::find_if(std::begin(map), std::end(map), [](auto const &field) {
           return std::holds_alternative<Key>(field.second);
         }) != std::end(map);
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

bool nextKeys(std::map<MapPosition, int> &keys, VaultMap const &map,
              std::vector<MapPosition> const &visited,
              MapPosition const &current, MapPosition const &direction,
              int count) {
  if (std::find(std::begin(visited), std::end(visited), current) !=
      std::end(visited))
    return false;
  // std::cout << ".";
  auto nextVisited{visited};
  nextVisited.push_back(current);

  if (!isKey(map, current)) {
    count++;
    if (isReachable(map, current + right(direction)))
      nextKeys(keys, map, nextVisited, current + right(direction),
               right(direction), count);

    if (isReachable(map, current + left(direction)))
      nextKeys(keys, map, nextVisited, current + left(direction),
               left(direction), count);

    if (isReachable(map, current + direction))
      nextKeys(keys, map, nextVisited, current + direction, direction, count);

    if (isReachable(map, current + reverse(direction)))
      nextKeys(keys, map, nextVisited, current + reverse(direction),
               reverse(direction), count);
  } else {
    keys[current] = count;
    return true;
  }

  return false;
}

void allfrom(std::map<int, int> &steps,
             std::map<int, std::map<MapPosition, int>> &keyPaths, VaultMap map,
             std::map<char, MapPosition> const &doors, MapPosition const &start,
             Path path, int count) {

  if (count > 3090)
    return;
  if (!std::holds_alternative<Key>(map.at(start)))
    return;

  auto key = std::get<Key>(map.at(start)).key;
  map[start] = Open{};
  if (doors.count(key))
    map[doors.at(key)] = Open{};

  int abstractPath = key * 10000;
  for (auto const &c : path)
    abstractPath += c;

  std::map<MapPosition, int> keys;
  if (keyPaths.count(abstractPath))
    keys = keyPaths.at(abstractPath);
  else {
    nextKeys(keys, map, {}, start, {-1, 0}, 0);
    keyPaths[abstractPath] = keys;
  }

  path.push_back(key);

  if (keys.empty()) {
    for (auto const &c : path)
      std::cout << c;
    std::cout << ": " << count << "\n";
    if (!steps.count(-1) || count < steps[-1])
      steps[-1] = count;
    return;
  }

  if (!steps.count(abstractPath) || count < steps[abstractPath]) {
    std::for_each(std::begin(keys), std::end(keys), [&](auto const &key) {
      allfrom(steps, keyPaths, map, doors, key.first, path, count + key.second);
    });
    steps[abstractPath] = count;
  }
}

} // namespace

int Vault::collectKeys() {
  auto entrance = findEntrance(vault_);
  VaultMap map = vault_;
  map[entrance] = Open{};

  std::map<int, int> steps;
  std::map<MapPosition, int> keys;
  auto doors = allDoors(map);

  nextKeys(keys, map, {}, entrance, {-1, 0}, 0);
  std::map<int, std::map<MapPosition, int>> keyPaths;
  std::for_each(std::begin(keys), std::end(keys), [&](auto const &key) {
    allfrom(steps, keyPaths, map, doors, key.first, {}, key.second);
  });

  return steps.begin()->second;
  //*std::min_element(std::begin(steps.begin()->second),
  // std::end(steps.begin()->second));
}