#include <maze.h>

#include <algorithm>
#include <map>
#include <tuple>

using namespace AdventOfCode;

namespace {
const char OPEN = '.';
const char PORTAL = '+';
const char WALL = '#';
const char EMPTY = ' ';

auto horizontalPortal(Map &maze, MapPosition entry) {
  MapPosition portal;
  if (entry.x == 0)
    portal = {entry.x + 2, entry.y};
  else if (entry.x == maze.width() - 2)
    portal = {entry.x - 1, entry.y};
  else if (maze.at({entry.x + 2, entry.y}) == OPEN)
    portal = {entry.x + 2, entry.y};
  else
    portal = {entry.x - 1, entry.y};

  maze.set(entry, WALL);
  maze.set({entry.x + 1, entry.y}, WALL);
  maze.set(portal, PORTAL);

  return portal;
}
auto verticalPortal(Map &maze, MapPosition entry) {
  MapPosition portal;
  if (entry.y == 0)
    portal = {entry.x, entry.y + 2};
  else if (entry.y == maze.height() - 2)
    portal = {entry.x, entry.y - 1};
  else if (maze.at({entry.x, entry.y + 2}) == OPEN)
    portal = {entry.x, entry.y + 2};
  else
    portal = {entry.x, entry.y - 1};

  maze.set(entry, WALL);
  maze.set({entry.x, entry.y + 1}, WALL);
  maze.set(portal, PORTAL);

  return portal;
}

auto findPortals(Map &maze) {
  MapPosition start;
  MapPosition end;
  std::map<MapPosition, MapPosition> portals;
  std::map<std::tuple<char, char>, MapPosition> entries;

  for (int y = 0; y < maze.height() - 1; ++y) {
    for (int x = 0; x < maze.width() - 1; ++x) {
      auto p1 = maze.at({x, y});
      if (p1 >= 'A') {
        auto p2 = maze.at({x + 1, y});
        if (p2 >= 'A') {
          if (p1 == 'A' && p2 == 'A')
            start = horizontalPortal(maze, {x, y});
          else if (p1 == 'Z' && p2 == 'Z')
            end = horizontalPortal(maze, {x, y});
          else if (entries.count({p1, p2})) {
            auto exit = horizontalPortal(maze, {x, y});
            portals[exit] = entries[{p1, p2}];
            portals[entries[{p1, p2}]] = exit;
          } else
            entries[{p1, p2}] = horizontalPortal(maze, {x, y});
        } else {
          p2 = maze.at({x, y + 1});
          if (p1 == 'A' && p2 == 'A')
            start = verticalPortal(maze, {x, y});
          else if (p1 == 'Z' && p2 == 'Z')
            end = verticalPortal(maze, {x, y});
          else if (entries.count({p1, p2})) {
            auto exit = verticalPortal(maze, {x, y});
            portals[exit] = entries[{p1, p2}];
            portals[entries[{p1, p2}]] = exit;
          } else
            entries[{p1, p2}] = verticalPortal(maze, {x, y});
        }
      }
    }
  }

  maze.print();
  return std::tuple{start, end, portals};
}

int findExit(Map maze, std::vector<MapPosition> visited,
             MapPosition const &start, MapPosition const &end,
             std::map<MapPosition, MapPosition> const &portals) {
  maze.set(start, OPEN);
  auto reachable = maze.find({PORTAL}, start);

  int minimum = 1000;
  for (auto const &pos : reachable) {
    if (std::find(std::begin(visited), std::end(visited), pos.first) !=
        std::end(visited))
      continue;
      
    visited.push_back(pos.first);
    int length = maze.shortestPath(start, pos.first);
    if (pos.first != end)
      length +=
          findExit(maze, visited, portals.at(pos.first), end, portals) + 1;

    if (length < minimum)
      minimum = length;
  }
  return minimum;
}
} // namespace

Maze::Maze(std::istream &map) : maze_{map} {}

int Maze::shortestPath() {
  auto [start, end, portals] = findPortals(maze_);

  return findExit(maze_, {}, start, end, portals);
}