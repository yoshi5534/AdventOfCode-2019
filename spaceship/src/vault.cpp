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
using Path = std::string;

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

auto allKeys(VaultMap const &map) {
  Path keys;
  std::for_each(std::begin(map), std::end(map), [&](auto const &field) {
    if (std::holds_alternative<Key>(field.second)) {
      keys.push_back(std::get<Key>(field.second).key);
    }
  });

  return keys;
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

int allfrom(std::map<Path, int> &steps,
            std::map<Path, std::map<MapPosition, int>> &keyPaths, VaultMap map,
            Path const &allKeys, std::map<char, MapPosition> const &doors,
            MapPosition const &start, Path path, int count) {
  if (!std::holds_alternative<Key>(map.at(start)))
    return 100000;

  auto key = std::get<Key>(map.at(start)).key;
  map[start] = Open{};
  if (doors.count(key))
    map[doors.at(key)] = Open{};

  auto abstractPath = path;
  std::sort(std::begin(abstractPath), std::end(abstractPath));
  abstractPath.push_back(key);

  std::map<MapPosition, int> keys;
  if (keyPaths.count(abstractPath))
    keys = keyPaths.at(abstractPath);
  else {
    nextKeys(keys, map, {}, start, {-1, 0}, 0);
    keyPaths[abstractPath] = keys;
  }

  path.push_back(key);

  if (keys.empty()) {
    // for (auto const &c : path)
    //   std::cout << c;
    // std::cout << ": " << count << "\n";
    if (!steps.count(".") || count < steps["."])
      steps["."] = count;
    return count;
  }

  if (!steps.count(abstractPath)) {
    int minimum = 1000000;
    std::for_each(std::begin(keys), std::end(keys), [&](auto const &key) {
      auto length = allfrom(steps, keyPaths, map, allKeys, doors, key.first,
                            path,  key.second);
      if (length < minimum)
        minimum = length;
    });
    steps[abstractPath] = minimum;
  }

  return steps[abstractPath] + count;
}

} // namespace

int Vault::collectKeys() {
  auto entrance = findEntrance(vault_);
  VaultMap map = vault_;
  map[entrance] = Open{};

  std::map<Path, int> steps;
  std::map<MapPosition, int> keys;
  auto doors = allDoors(map);
  auto allkeys = allKeys(map);

  nextKeys(keys, map, {}, entrance, {1, 0}, 0);
  std::map<Path, std::map<MapPosition, int>> keyPaths;
  int minimum = 1000000;
  std::for_each(std::begin(keys), std::end(keys), [&](auto const &key) {
    auto length = allfrom(steps, keyPaths, map, allkeys, doors, key.first, {},
                          key.second);
    if (length < minimum)
      minimum = length;
  });

  return minimum;
}