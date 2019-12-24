#pragma once

#include <computer.h>

#include <map>
#include <optional>
#include <tuple>
#include <variant>

namespace AdventOfCode {

class ArcadeCabinet {
public:
  ArcadeCabinet(Program const &program);

  void play();
  int getScore () const;

private:
  void updateScore();

private:
  Computer computer_;
  int score_;
};

} // namespace AdventOfCode
