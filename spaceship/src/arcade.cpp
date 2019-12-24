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
}
  ArcadeCabinet::ArcadeCabinet(Program const &program)
      : computer_{program}, screen_{}, score_{0}, paddlePosition_{0, 0},
        ballPosition_{0, 0}, direction_{1, -1} {}

  void ArcadeCabinet::play() {
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

  int ArcadeCabinet::getScore () const {return score_; }

  int ArcadeCabinet::getNextOutput() {
    Intcode code;
    do {
      code = computer_.runInstruction();
    } while (code != Intcode::Output && code != Intcode::Halt);
    if (code == Intcode::Halt)
      return -99;

    return computer_.readOutput();
  }

  std::optional<Tile> ArcadeCabinet::getNextTile() {
    auto x = getNextOutput();
    if (x < 0)
      return std::nullopt;

    auto y = getNextOutput();
    auto tileId = getTileId(getNextOutput());

    return Tile{{x, y}, tileId};
  }
  
  void ArcadeCabinet::moveJoystick() {
    if (paddlePosition_ == ballPosition_)
      computer_.writeInput({direction_.x});
    if (paddlePosition_.x < ballPosition_.x + direction_.x)
      computer_.writeInput({1});
    else if (paddlePosition_.x > ballPosition_.x + direction_.x)
      computer_.writeInput({-1});
    else
      computer_.writeInput({direction_.x});
  }

  void ArcadeCabinet::updateScore() {
    auto y = getNextOutput();
    if (y != 0)
      throw std::runtime_error("should be zero for segment");

    score_ = getNextOutput();
  }

  auto ArcadeCabinet::getScreenDimension() const {
    std::vector<int> xValues;
    std::vector<int> yValues;
    std::for_each(std::begin(screen_), std::end(screen_), [&](auto tile) {
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

  auto ArcadeCabinet::predictDirection(ScreenPosition newBallPosition) const {
    printScreen();
    [[maybe_unused]] auto [width, height] = getScreenDimension();
    ScreenPosition direction{direction_};

    if(newBallPosition. x > ballPosition_.x)
      direction.x = 1;
    else if(newBallPosition. x < ballPosition_.x)
      direction.x = -1;

    if(newBallPosition. y > ballPosition_.y)
      direction.y = 1;

    if (newBallPosition.x == 1)
      direction.x = 1;
    if (newBallPosition.x == width - 2)
      direction.x = -1;

    if (newBallPosition.y == 1)
      direction.y = 1;
    if (newBallPosition.y == height - 3)
      direction.y = -1;

    auto hittedTile = screen_.find(newBallPosition + direction);
    auto neighbourBall = screen_.find({newBallPosition.x + direction.x, newBallPosition.y});
    auto aboveBall = screen_.find({newBallPosition.x, newBallPosition.y + direction.y});
    if (std::holds_alternative<Block>(hittedTile->second))
    {
      if (std::holds_alternative<Empty>(neighbourBall->second))
        direction.y = direction.y * -1;
      else if (std::holds_alternative<Block>(aboveBall->second))
        direction.y = direction.y * -1;

      if (std::holds_alternative<Empty>(aboveBall->second))
        direction.x = direction_.x * -1;
      else if (std::holds_alternative<Block>(neighbourBall->second))
        direction.x = direction_.x * -1;
     }
    else if (std::holds_alternative<Block>(neighbourBall->second))
    {      
      auto aboveBall = screen_.find({newBallPosition.x, newBallPosition.y + direction.y});
      auto nextHitBall = screen_.find({newBallPosition.x - direction.x, newBallPosition.y + direction.y});
      // #
      //#@  
      if (std::holds_alternative<Block>(aboveBall->second))
        direction.x = direction_.x * -1;
      //  #
      //#@  
      else if (std::holds_alternative<Block>(nextHitBall->second))
      { // direction is kept except
        // #
        // #@
        //   #
        auto oppositeHit = screen_.find({newBallPosition.x + direction.x, newBallPosition.y - direction.y});
        if (std::holds_alternative<Block>(oppositeHit->second))
          direction.x = direction_.x * -1;
      }
      //#
      //#@
      else
        direction.x = direction_.x * -1;
    }

    return direction;
  }

  void ArcadeCabinet::drawTile(int x) {
    auto y = getNextOutput();
    auto tileId = getTileId(getNextOutput());
    screen_[{x, y}] = tileId;
    if (std::holds_alternative<Paddle>(tileId))
      paddlePosition_ = {x,y};
    if (std::holds_alternative<Ball>(tileId)) {
      direction_ = predictDirection({x,y});
      ballPosition_ = {x,y};
    }
  }


  void ArcadeCabinet::printScreen() const {
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
