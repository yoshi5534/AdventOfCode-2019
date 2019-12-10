#pragma once

#include <computer.h>

#include <vector>

namespace AdventOfCode {

struct AmplifierChain {
  static int maxThrusterSignal(Program const &program);
};

class FeedbackLoop {
public:
  FeedbackLoop(Program const &program);

  int maxOutputSignal ();
private:
  int outputSignal (Input const& phaseSetting);

  std::vector <Computer> amplifiers_;
};
} // namespace AdventOfCode