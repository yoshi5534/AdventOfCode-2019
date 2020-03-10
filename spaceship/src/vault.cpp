#include <vault.h>

#include <algorithm>
#include <iostream>
#include <set>
#include <sstream>
#include <vector>

using namespace AdventOfCode;

namespace {
using Path = std::string;
const char WALL = '#';
const char OPEN = '.';
const char ENTRANCE = '@';

std::vector<char> allKeys() {
  std::vector<char> all(26);

  std::generate(std::begin(all), std::end(all),
                [c = 'a']() mutable { return c++; });
  return all;
}

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

auto findEntrances(Map &map) {
  std::vector<MapPosition> positions;

  int robot = 0;
  for (int y = 0; y < map.height(); ++y)
    for (int x = 0; x < map.width(); ++x)
      if (map.at({x, y}) == ENTRANCE) {
        positions.push_back({x, y});
        map.set({x, y}, '0' + robot++);
      }

  return positions;
}

auto allDoors(Map const &map) {
  std::map<char, MapPosition> doors;

  for (int y = 0; y < map.height(); ++y)
    for (int x = 0; x < map.width(); ++x)
      if (map.at({x, y}) >= 'A' && map.at({x, y}) <= 'Z')
        doors[std::tolower(map.at({x, y}))] = {x, y};

  return doors;
}

int allfrom(std::map<Path, int> &steps,
            std::map<Path, std::map<MapPosition, char>> &keyPaths, Map map,
            std::vector<MapPosition> robots,
            std::map<Path, int> &allKeyDistances,
            std::map<char, MapPosition> const &doors, MapPosition const &start,
            Path path, int count) {

  auto key = map.at(start);
  int startQuadrant = 0;
  if (robots.size() == 4)
    startQuadrant = quadrant(start, map.width(), map.height());

  robots[startQuadrant] = start;

  for (auto const &robot : robots)
    map.set(robot, OPEN);

  if (doors.count(key))
    map.set(doors.at(key), OPEN);

  auto abstractPath = path;
  std::sort(std::begin(abstractPath), std::end(abstractPath));
  abstractPath.push_back(key);

  std::map<MapPosition, char> keys;
  if (keyPaths.count(abstractPath))
    keys = keyPaths.at(abstractPath);
  else {
    for (auto const &robot : robots) {
      auto reachableKeys = map.find(allKeys(), robot);
      keys.insert(std::begin(reachableKeys), std::end(reachableKeys));
    }
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
        quad = quadrant(nextKey.first, map.width(), map.height());

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
        distance = map.shortestPath(robots[quad], nextKey.first);
        allKeyDistances[nextKeyPath] = distance;
        allKeyDistances[backKeyPath] = distance;
      }

      auto const length = allfrom(steps, keyPaths, map, robots, allKeyDistances,
                                  doors, nextKey.first, path, distance);

      if (length < minimum)
        minimum = length;
    });
    steps[abstractPath] = minimum;
  }

  return steps[abstractPath] + count;
}

} // namespace

Vault::Vault(std::istream &map) : vault_{map} {}

int Vault::collectKeys() const {
  Map map = vault_;
  auto entrances = findEntrances(map);
  auto doors = allDoors(map);
  for(auto const& door: doors)
    map.set (door.second, WALL);

  std::map<Path, int> steps;
  std::map<Path, int> allKeyDistances;
  std::map<Path, std::map<MapPosition, char>> keyPaths;

  auto length = allfrom(steps, keyPaths, map, entrances, allKeyDistances, doors,
                        entrances[0], {}, 0);

  return length;
}

void Vault::deployRobots() {
  auto entrance = *findEntrances(vault_).begin();
  vault_.set(entrance + MapPosition{-1, -1}, ENTRANCE);
  vault_.set(entrance + MapPosition{0, -1}, WALL);
  vault_.set(entrance + MapPosition{1, -1}, ENTRANCE);
  vault_.set(entrance + MapPosition{-1, 0}, WALL);
  vault_.set(entrance + MapPosition{0, 0}, WALL);
  vault_.set(entrance + MapPosition{1, 0}, WALL);
  vault_.set(entrance + MapPosition{-1, 1}, ENTRANCE);
  vault_.set(entrance + MapPosition{0, 1}, WALL);
  vault_.set(entrance + MapPosition{1, 1}, ENTRANCE);
}