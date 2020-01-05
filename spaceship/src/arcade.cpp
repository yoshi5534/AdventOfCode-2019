#include <arcade.h>

#include <computer.h>

#include <algorithm>
#include <exception>
#include <iostream>
#include <map>
#include <optional>
#include <tuple>
#include <variant>

using namespace AdventOfCode;

namespace {
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

struct GameState {
  GameState() : paddlePosition{0, 0}, ballPosition{0, 0}, direction{0, -1} {}

  ScreenPosition paddlePosition;
  ScreenPosition ballPosition;
  ScreenPosition direction;
};

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

int getNextOutput(Computer &computer) {
  Intcode code;
  do {
    code = computer.runInstruction();
  } while (code != Intcode::Output && code != Intcode::Halt);
  if (code == Intcode::Halt)
    return -99;

  return computer.readOutput();
}

void moveJoystick(Computer &computer, GameState &state) {
  if (state.paddlePosition == state.ballPosition)
    computer.writeInput({state.direction.x});
  if (state.paddlePosition.x < state.ballPosition.x + state.direction.x)
    computer.writeInput({1});
  else if (state.paddlePosition.x > state.ballPosition.x + state.direction.x)
    computer.writeInput({-1});
  else
    computer.writeInput({state.direction.x});
}

auto getScreenDimension(Screen &screen) {
  std::vector<int> xValues;
  std::vector<int> yValues;
  std::for_each(std::begin(screen), std::end(screen), [&](auto tile) {
    auto pos = tile.first;
    xValues.push_back(pos.x);
    yValues.push_back(pos.y);
  });

  auto &&[minX, maxX] = std::minmax_element(begin(xValues), end(xValues));
  auto &&[minY, maxY] = std::minmax_element(begin(yValues), end(yValues));

  int width = *maxX - *minX + 1;
  int height = *maxY - *minY + 1;

  return std::make_pair(width, height);
}

void printScreen(Screen &screen, int score) {
  auto [width, height] = getScreenDimension(screen);
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      auto tile = screen.find({x, y});
      if (tile != std::end(screen)) {
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
  std::cout << "\n   SCORE   " << score << std::endl;
}

void predictDirection(Screen &screen, GameState &state,
                      ScreenPosition newBallPosition) {
  auto [width, height] = getScreenDimension(screen);
  ScreenPosition direction{state.direction};

  if (newBallPosition.x > state.ballPosition.x)
    direction.x = 1;
  else if (newBallPosition.x < state.ballPosition.x)
    direction.x = -1;

  if (newBallPosition.y > state.ballPosition.y)
    direction.y = 1;

  if (newBallPosition.x == 1)
    direction.x = 1;
  if (newBallPosition.x == width - 2)
    direction.x = -1;

  if (newBallPosition.y == 1)
    direction.y = 1;
  if (newBallPosition.y == height - 3)
    direction.y = -1;

  auto hittedTile = screen.find(newBallPosition + direction);
  auto neighbourBall =
      screen.find({newBallPosition.x + direction.x, newBallPosition.y});
  auto aboveBall =
      screen.find({newBallPosition.x, newBallPosition.y + direction.y});
  if (std::holds_alternative<Block>(hittedTile->second)) {
    if (std::holds_alternative<Empty>(neighbourBall->second))
      direction.y = direction.y * -1;
    else if (std::holds_alternative<Block>(aboveBall->second))
      direction.y = direction.y * -1;

    if (std::holds_alternative<Empty>(aboveBall->second))
      direction.x = state.direction.x * -1;
    else if (std::holds_alternative<Block>(neighbourBall->second))
      direction.x = state.direction.x * -1;
  } else if (std::holds_alternative<Block>(neighbourBall->second)) {
    auto aboveBall =
        screen.find({newBallPosition.x, newBallPosition.y + direction.y});
    auto nextHitBall = screen.find(
        {newBallPosition.x - direction.x, newBallPosition.y + direction.y});
    // #
    //#@
    if (std::holds_alternative<Block>(aboveBall->second))
      direction.x = state.direction.x * -1;
    //  #
    //#@
    else if (std::holds_alternative<Block>(
                 nextHitBall->second)) { // direction is kept except
      // #
      // #@
      //   #
      auto oppositeHit = screen.find(
          {newBallPosition.x + direction.x, newBallPosition.y - direction.y});
      if (std::holds_alternative<Block>(oppositeHit->second))
        direction.x = state.direction.x * -1;
    }
    //#
    //#@
    else
      direction.x = state.direction.x * -1;
  }

  state.direction = direction;
}

void drawTile(Computer &computer, Screen &screen, GameState &state, int x) {
  auto y = getNextOutput(computer);
  auto tileId = getTileId(getNextOutput(computer));
  screen[{x, y}] = tileId;
  if (std::holds_alternative<Paddle>(tileId))
    state.paddlePosition = {x, y};
  if (std::holds_alternative<Ball>(tileId)) {
    predictDirection(screen, state, {x, y});
    state.ballPosition = {x, y};
  }
}

} // namespace
ArcadeCabinet::ArcadeCabinet(Program const &program)
    : computer_{program}, score_{0} {}

void ArcadeCabinet::play() {
  Screen screen{};
  GameState state{};
  computer_.writeInput({0});
  while (true) {
    Intcode code;
    do {
      code = computer_.runInstruction();
    } while (code != Intcode::Output && code != Intcode::Input &&
             code != Intcode::Halt);
    if (code == Intcode::Halt)
      break;
    if (code == Intcode::Input) {
      moveJoystick(computer_, state);
      printScreen(screen, score_);
    }
    if (code == Intcode::Output) {
      auto output = computer_.readOutput();
      if (output == -1)
        updateScore();
      else
        drawTile(computer_, screen, state, output);
    }
  }
}

int ArcadeCabinet::getScore() const { return score_; }

void ArcadeCabinet::updateScore() {
  auto y = getNextOutput(computer_);
  if (y != 0)
    throw std::runtime_error("should be zero for segment");

  score_ = getNextOutput(computer_);
}