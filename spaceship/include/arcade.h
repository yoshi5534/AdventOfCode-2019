#pragma once

#include <computer.h>

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