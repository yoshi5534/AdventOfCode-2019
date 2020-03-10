#include <map.h>

#include <algorithm>
#include <set>
#include <sstream>

using namespace AdventOfCode;

namespace {
char const WALL = '#';

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

bool isReachable(Map const &map, MapPosition const &field) {
  return map.at(field) != WALL;
}

MapPosition getDelta(MapPosition const &source, MapPosition const &target) {
  return {abs(source.x - target.x), abs(source.y - target.y)};
}

uint manhattan(MapPosition const &source, MapPosition const &target) {
  auto delta = getDelta(source, target);
  return static_cast<uint>(10 * (delta.x + delta.y));
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

bool findItems(Positions &positions, Map const &map,
               std::vector<char> const &items, std::vector<MapPosition> visited,
               MapPosition const &current, MapPosition const &direction) {
  if (std::find(std::begin(visited), std::end(visited), current) !=
      std::end(visited))
    return false;

  visited.push_back(current);

  if (std::find(std::begin(items), std::end(items), map.at(current)) ==
      std::end(items)) {
    if (isReachable(map, current + right(direction)))
      findItems(positions, map, items, visited, current + right(direction),
                right(direction));

    if (isReachable(map, current + left(direction)))
      findItems(positions, map, items, visited, current + left(direction),
                left(direction));

    if (isReachable(map, current + direction))
      findItems(positions, map, items, visited, current + direction, direction);

    if (isReachable(map, current + reverse(direction)))
      findItems(positions, map, items, visited, current + reverse(direction),
                reverse(direction));
  } else {
    positions[current] = map.at(current);
    return true;
  }

  return false;
}

} // namespace

Map::Map(std::istream &map) : fields_{}, width_{}, height_{} {
  std::string line;

  int height = 0;
  int width = 0;
  while (std::getline(map, line)) {
    width = 0;
    for (char c : line) {
      fields_.emplace_back(c);
      width++;
    }
    height++;
  }

  width_ = width;
  height_ = height;
}

std::vector<char> &Map::fields() { return fields_; }
int Map::width() const { return width_; }
int Map::height() const { return height_; }

char Map::at(MapPosition const &pos) const {
  return fields_[pos.y * width_ + pos.x];
}

void Map::set(MapPosition const &pos, char c) {
  fields_[pos.y * width_ + pos.x] = c;
}

int Map::shortestPath(MapPosition const &start, MapPosition const &end) const {
  std::vector<MapPosition> directions = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};

  Node *current = nullptr;
  NodeSet openSet, closedSet;
  openSet.insert(new Node(start));
  while (!openSet.empty()) {
    current = *openSet.begin();
    for (auto const &node : openSet) {
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
      if (!isReachable(*this, newCoordinates) ||
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
  while ((current = current->parent_) != nullptr)
    path++;

  releaseNodes(openSet);
  releaseNodes(closedSet);

  return path;
}

Positions Map::find(std::vector<char> const &items,
                    MapPosition const &start) const {
  Positions positions;
  findItems(positions, *this, items, {}, start, {1, 0});
  return positions;
}