#pragma once

#include <computer.h>

#include <algorithm>
#include <exception>
#include <map>
#include <optional>
#include <tuple>
#include <variant>

namespace AdventOfCode {

using ScreenPosition = std::pair <int, int>;

struct Empty {};
struct Wall {};
struct Block {};
struct Paddle {};
struct Ball {};

using TileId = std::variant<Empty, Wall, Block, Paddle, Ball>;
using Tile = std::pair<ScreenPosition, TileId>;

class ArcadeCabinet {
  public:
  ArcadeCabinet(Program const& program) : computer_{program} {}

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

    throw std::runtime_error ("no known tile id");
  }

  std::optional<Tile> getNextTile() {
    auto x = getNextOutput();
    if (x < 0)
      return std::nullopt;

    auto y = getNextOutput();
    auto tileId = getTileId(getNextOutput());

    return Tile {{x, y}, tileId};
  }

  int drawTiles() {
    std::map<ScreenPosition, TileId> screen;
    while (true) {
      auto tile = getNextTile();
      if (!tile)
        break;

      screen[tile->first] = tile->second;
    }

    return std::count_if (std::begin(screen), std::end (screen), [](auto const& tile) {
      return std::holds_alternative<Block> (tile.second);
    });
  }

private:
  Computer computer_;
};

} // namespace AdventOfCode
