#pragma once

#include <computer.h>

#include <algorithm>
#include <exception>
#include <iostream>
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
  ArcadeCabinet(Program const &program)
      : computer_{program}, screen_{}, score_{0}, paddlePosition_{0},
        ballPosition_{0}, direction_{1} {}

  int getNextOutput() {
    Intcode code;
    do {
      code = computer_.runInstruction();
    } while (code != Intcode::Output && code != Intcode::Halt);
    if (code == Intcode::Halt)
      return -99;

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

  int getId(TileId tile) {
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

  void moveJoystick() {
    printScreen();
    if (paddlePosition_ == ballPosition_)
      computer_.writeInput({direction_});
    if (paddlePosition_ < ballPosition_ + direction_)
      computer_.writeInput({1});
    else if (paddlePosition_ > ballPosition_ + direction_)
      computer_.writeInput({-1});
    else
      computer_.writeInput({direction_});
  }

  void updateScore() {
    auto y = getNextOutput();
    if (y != 0)
      throw std::runtime_error("should be zero for segment");

    score_ = getNextOutput();
  }

  void drawTile(int x) {
    auto y = getNextOutput();
    auto tileId = getTileId(getNextOutput());
    screen_[{x, y}] = tileId;
    if (std::holds_alternative<Paddle>(tileId))
      paddlePosition_ = x;
    if (std::holds_alternative<Ball>(tileId))
    {
      if (x < ballPosition_)
        direction_ = -1;
      else if (x > ballPosition_)
        direction_ = 1;
      ballPosition_ = x;
    }
  }

  void play() {
    computer_.writeInput({0});
    while (true) {
      Intcode code;
      do {
        code = computer_.runInstruction();
      } while (code != Intcode::Output && code != Intcode::Input &&
               code != Intcode::Halt);
      if (code == Intcode::Halt)
        break;
      if (code == Intcode::Input)
        moveJoystick();
      if (code == Intcode::Output) {
        auto output = computer_.readOutput();
        if (output == -1)
          updateScore();
        else
          drawTile(output);
      }
    }
  }

  auto getScreenDimension() {
    std::vector<int> xValues;
    std::vector<int> yValues;
    std::for_each(std::begin(screen_), std::end(screen_), [&](auto tile) {
      auto pos = tile.first;
      xValues.push_back(pos.first);
      yValues.push_back(pos.second);
    });

    auto &&[minX, maxX] = std::minmax_element(begin(xValues), end(xValues));
    auto &&[minY, maxY] = std::minmax_element(begin(yValues), end(yValues));

    int width = *maxX - *minX + 1;
    int height = *maxY - *minY + 1;

    return std::make_pair(width, height);
  }

  void printScreen() {
    auto [width, height] = getScreenDimension();
    for (int y = 0; y < height; ++y) {
      for (int x = 0; x < width; ++x) {
        auto tile = screen_.find({x, y});
        if (tile != std::end(screen_)) {
          if (std::holds_alternative<Empty>(tile->second))
            std::cout << ' ';
          if (std::holds_alternative<Wall>(tile->second))
            std::cout << 'W';
          if (std::holds_alternative<Block>(tile->second))
            std::cout << '#';
          if (std::holds_alternative<Paddle>(tile->second))
            std::cout << '-';
          if (std::holds_alternative<Ball>(tile->second))
            std::cout << '@';
        } else
          std::cout << ' ';
      }
      std::cout << '\n';
    }
  }

private:
  Computer computer_;
  Screen screen_;
  int score_;
  int paddlePosition_;
  int ballPosition_;
  int direction_;
};

} // namespace AdventOfCode
