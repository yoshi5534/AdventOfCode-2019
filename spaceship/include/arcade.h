#pragma once

#include <computer.h>

#include <map>
#include <optional>
#include <tuple>
#include <variant>

namespace AdventOfCode {

struct ScreenPosition {
  int x;
  int y;

  ScreenPosition operator+(ScreenPosition const &other) const {
    return {x + other.x, y + other.y};
  }
  bool operator==(ScreenPosition const &other) const {
    return x == other.x && y == other.y;
  }

  constexpr bool operator<(ScreenPosition const &other) const {
    if (y < other.y)
      return true;
    if (y == other.y)
      return x < other.x;

    return false;
  }
};

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
  ArcadeCabinet(Program const &program);

  void play();
  int getScore () const;

private:
  int getNextOutput();
  std::optional<Tile> getNextTile();
  
  void moveJoystick();
  void updateScore();

  auto getScreenDimension() const;

  auto predictDirection(ScreenPosition newBallPosition) const;

  void drawTile(int x);

  void printScreen() const;

private:
  Computer computer_;
  Screen screen_;
  int score_;
  ScreenPosition paddlePosition_;
  ScreenPosition ballPosition_;
  ScreenPosition direction_;
};

} // namespace AdventOfCode
