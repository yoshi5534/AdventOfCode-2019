#pragma once

#include <computer.h>
#include <spaceimage.h>

#include <algorithm>
#include <exception>
#include <map>
#include <optional>
#include <tuple>
#include <variant>

namespace AdventOfCode {

using ScreenPosition = std::pair<int, int>;

struct Empty {};
struct Wall {};
struct Block {};
struct Paddle {};
struct Ball {};

using TileId = std::variant<Empty, Wall, Block, Paddle, Ball>;
using Tile = std::pair<ScreenPosition, TileId>;
using Screen = std::map<ScreenPosition, TileId>;

class ArcadeCabinet {
public:
  ArcadeCabinet(Program const &program) : computer_{program} {}

  int getNextOutput() {
    Intcode code;
    do {
      code = computer_.runInstruction();
    } while (code != Intcode::Output && code != Intcode::Halt);
    if (code == Intcode::Halt)
      return -1;

    return computer_.readOutput();
  }

  TileId getTileId(int id) {
    if (id == 0)
      return Empty{};
    if (id == 1)
      return Wall{};
    if (id == 2)
      return Block{};
    if (id == 3)
      return Paddle{};
    if (id == 4)
      return Ball{};

    throw std::runtime_error("no known tile id");
  }

  int getId (TileId tile)
  {
    if (std::holds_alternative<Empty>(tile))
      return 0;
    if (std::holds_alternative<Wall>(tile))
      return 1;
    if (std::holds_alternative<Block>(tile))
      return 2;
    if (std::holds_alternative<Paddle>(tile))
      return 3;
    if (std::holds_alternative<Ball>(tile))
      return 4;

    throw std::runtime_error("no known tile id");
  }

  std::optional<Tile> getNextTile() {
    auto x = getNextOutput();
    if (x < 0)
      return std::nullopt;

    auto y = getNextOutput();
    auto tileId = getTileId(getNextOutput());

    return Tile{{x, y}, tileId};
  }

  Screen drawTiles() {
    Screen screen;
    while (true) {
      auto tile = getNextTile();
      if (!tile)
        break;

      screen[tile->first] = tile->second;
    }

    return screen;
  }

  SpaceImage getScreen() {
    auto screen = drawTiles ();

    std::vector<int> xValues;
    std::vector<int> yValues;
    std::for_each(std::begin(screen), std::end(screen), [&](auto tile) {
      auto pos = tile.first;
      xValues.push_back(pos.first);
      yValues.push_back(pos.second);
    });

    auto &&[minX, maxX] = std::minmax_element(begin(xValues), end(xValues));
    auto &&[minY, maxY] = std::minmax_element(begin(yValues), end(yValues));

    int width = *maxX - *minX + 1;
    int height = *maxY - *minY + 1;

    Format data(width * height);

    std::for_each(std::begin(screen), std::end(screen), [&](auto tile) {
      auto pos = tile.first;
      auto x = std::abs(pos.first);
      auto y = std::abs(pos.second);
      int id = getId (tile.second);

      data[y * width + x] = id;
    });

    return SpaceImage{width, height, data};
  }

private:
  Computer computer_;
};

} // namespace AdventOfCode
