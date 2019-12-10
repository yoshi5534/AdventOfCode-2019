#include <amplifier.h>

#include <algorithm>

using namespace AdventOfCode;

namespace {
int getSignal(Input phaseSetting, Program const &program) {
  int signal = 0;
  for (int i = 0; i < 5; ++i) {
    Computer amplifier(program);
    amplifier.writeInput({phaseSetting[i]});
    amplifier.writeInput({signal});
    amplifier.calculate();
    signal = {amplifier.readOutput()};
  }

  return signal;
}
} // namespace

int AmplifierChain::maxThrusterSignal(Program const &program) {
  Input phaseSetting{0, 1, 2, 3, 4};

  int maxThrusterSignal = 0;
  do {
    int signal = getSignal(phaseSetting, program);
    if (signal > maxThrusterSignal)
      maxThrusterSignal = signal;
  } while (
      std::next_permutation(std::begin(phaseSetting), std::end(phaseSetting)));

  return maxThrusterSignal;
}

FeedbackLoop::FeedbackLoop(Program const &program) {
  for (int i = 0; i < 5; ++i)
    amplifiers_.push_back(Computer{program});
}

int FeedbackLoop::maxOutputSignal() { return 0; }

int FeedbackLoop::outputSignal(Input const &phaseSetting) { return 0; }