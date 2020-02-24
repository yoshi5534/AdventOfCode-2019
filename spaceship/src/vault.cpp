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
              std::vector<MapPosition> const &nextVisited,
              MapPosition const &current, MapPosition const &direction,
              int count) {
  // auto nextVisited{visited};
  // nextVisited.push_back(current);

  if (!isKey(map, current)) {
    count++;
    if (std::end(nextVisited) == std::find(std::begin(nextVisited),
                                           std::end(nextVisited),
                                           current + right(direction)) &&
        isReachable(map, current + right(direction)))
      nextKeys(keys, map, nextVisited, current + right(direction),
               right(direction), count);

    if (std::end(nextVisited) == std::find(std::begin(nextVisited),
                                           std::end(nextVisited),
                                           current + left(direction)) &&
        isReachable(map, current + left(direction)))
      nextKeys(keys, map, nextVisited, current + left(direction),
               left(direction), count);

    // if (std::end(nextVisited) == std::find(std::begin(nextVisited),
    //                                        std::end(nextVisited),
    //                                        current + reverse(direction)) &&
    //     isReachable(map, current + reverse(direction)))
    //   nextKeys(keys, map, nextVisited, current + reverse(direction),
    //            reverse(direction), count);

    if (std::end(nextVisited) == std::find(std::begin(nextVisited),
                                           std::end(nextVisited),
                                           current + direction) &&
        isReachable(map, current + direction))
      nextKeys(keys, map, nextVisited, current + direction, direction, count);
  } else {
    //auto key = std::get<Key>(map.at(current)).key;
    keys[current] = count;

    // std::cout << key << std::endl;
    return true;
  }

  return false;
}

void allfrom(std::vector<int> &steps, VaultMap map, MapPosition start,
             int count) {
  if (steps.size() &&
      count >= *std::min_element(std::begin(steps), std::end(steps)))
    return;

  auto key = std::get<Key>(map.at(start)).key;
  auto door =
      std::find_if(std::begin(map), std::end(map), [key](auto const &field) {
        return std::holds_alternative<Door>(field.second) &&
               std::get<Door>(field.second).door == std::toupper(key);
      });
  map[start] = Open{};
  if (door != std::end(map))
    map[door->first] = Open{};

  if (!hasKeys(map)) {
    std::cout << count << std::endl;
    steps.push_back(count);
    return;
  }

  std::map<MapPosition, int> keys;
  nextKeys(keys, map, {}, start, {-1, 0}, 0);
  nextKeys(keys, map, {}, start, {1, 0}, 0);

  std::for_each(std::begin(keys), std::end(keys), [&](auto const &key) {
    allfrom(steps, map, key.first, count + key.second);
  });
}

} // namespace

int Vault::collectKeys() {
  auto entrance = findEntrance(vault_);
  VaultMap map = vault_;
  map[entrance] = Open{};

  std::vector<int> steps;
  std::map<MapPosition, int> keys;
  nextKeys(keys, map, {}, entrance, {-1, 0}, 0);
  nextKeys(keys, map, {}, entrance, {1, 0}, 0);
  std::for_each(std::begin(keys), std::end(keys), [&](auto const &key) {
    allfrom(steps, map, key.first, key.second);
  });

  return *std::min_element(std::begin(steps), std::end(steps));
}