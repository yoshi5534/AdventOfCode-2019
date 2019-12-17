#include <robot.h>

#include <algorithm>
#include <map>
#include <tuple>
#include <type_traits>
#include <variant>

using namespace AdventOfCode;
namespace {
struct Up {};
struct Down {};
struct Left {};
struct Right {};

using Direction = std::variant<Up, Down, Left, Right>;
using Position = std::pair<int, int>;
using Grid = std::map<Position, Color>;

enum class Turning { Right, Left };
struct DirectionChanger {
  Direction &direction_;
  Turning &turn_;
  DirectionChanger(Direction &direction, Turning &turn)
      : direction_{direction}, turn_{turn} {}

  void operator()(Up) {
    if (turn_ == Turning::Right)
      direction_ = Right{};
    else {
      direction_ = Left{};
    }
  }
  void operator()(Down) {
    if (turn_ == Turning::Right)
      direction_ = Left{};
    else {
      direction_ = Right{};
    }
  }
  void operator()(Right) {
    if (turn_ == Turning::Right)
      direction_ = Down{};
    else {
      direction_ = Up{};
    }
  }
  void operator()(Left) {
    if (turn_ == Turning::Right)
      direction_ = Up{};
    else {
      direction_ = Down{};
    }
  }
};

struct Move {
  Position &position_;
  Move(Position &position) : position_{position} {}

  void operator()(Up) { position_.second++; }
  void operator()(Down) { position_.second--; }
  void operator()(Right) { position_.first++; }
  void operator()(Left) { position_.first--; }
};

Grid fill(Computer &computer, Color const &startColor) {
  Grid surface;
  Color color = startColor;
  Turning turning = Turning::Right;
  Intcode lastCode = Intcode::Input;
  Position position{0, 0};
  Direction direction{Up{}};

  surface[position] = color;
  while (!(lastCode == Intcode::Halt)) {
    if (surface.find(position) != std::end(surface))
      color = surface[position];
    else
      color = Color::Black;

    if (color == Color::White)
      computer.writeInput({1});
    else
      computer.writeInput({0});

    do {
      lastCode = computer.runInstruction();
    } while (lastCode != Intcode::Output && lastCode != Intcode::Halt);
    if (lastCode == Intcode::Halt)
      break;

    color = computer.readOutput() == 0 ? Color::Black : Color::White;
    surface[position] = color;

    do {
      lastCode = computer.runInstruction();
    } while (lastCode != Intcode::Output && lastCode != Intcode::Halt);
    if (lastCode == Intcode::Halt)
      break;

    turning = computer.readOutput() == 0 ? Turning::Left : Turning::Right;

    std::visit(DirectionChanger{direction, turning}, direction);
    std::visit(Move{position}, direction);
  }

  return surface;
}
} // namespace

EmergencyHullPaintingRobot::EmergencyHullPaintingRobot(Program const &program)
    : computer_{program} {}

SpaceImage EmergencyHullPaintingRobot::paint(Color const &startColor) {
  auto surface = fill(computer_, startColor);

  int minX = 0;
  int minY = 0;
  int maxX = 0;
  int maxY = 0;
  std::for_each(std::begin(surface), std::end(surface), [&](auto panel) {
    auto pos = panel.first;
    auto x = pos.first;
    auto y = pos.second;

    if (x < minX)
      minX = x;
    if (x > maxX)
      maxX = x;
    if (y < minY)
      minY = y;
    if (y > maxY)
      maxY = y;
  });

  int width = maxX - minX + 1;
  int height = maxY - minY + 1;

  Format data(width * height);

  std::for_each(std::begin(surface), std::end(surface), [&](auto panel) {
    auto pos = panel.first;
    auto x = std::abs(pos.first);
    auto y = std::abs(pos.second);
    int color = panel.second == Color::Black ? 0 : 1;

    data[y * width + x] = color;
  });

  return SpaceImage{width, height, data};
}