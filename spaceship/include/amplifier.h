#pragma once

#include <computer.h>

#include <vector>

namespace AdventOfCode {

struct AmplifierChain {
  static int64_t maxThrusterSignal(Program const &program);
};

class FeedbackLoop {
public:
  FeedbackLoop(Program const &program);

  int64_t maxOutputSignal ();
private:
  int64_t outputSignal (Input const& phaseSetting);

  std::vector <Computer> amplifiers_;
  Program program_;
};
} // namespace AdventOfCode