#include <amplifier.h>

#include <algorithm>

using namespace AdventOfCode;

namespace {
int64_t getSignal(Input phaseSetting, Program const &program) {
  int64_t signal = 0;
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

int64_t AmplifierChain::maxThrusterSignal(Program const &program) {
  Input phaseSetting{0, 1, 2, 3, 4};

  int64_t maxThrusterSignal = 0;
  do {
    int64_t signal = getSignal(phaseSetting, program);
    if (signal > maxThrusterSignal)
      maxThrusterSignal = signal;
  } while (
      std::next_permutation(std::begin(phaseSetting), std::end(phaseSetting)));

  return maxThrusterSignal;
}

FeedbackLoop::FeedbackLoop(Program const &program) : program_{program} {}

int64_t FeedbackLoop::maxOutputSignal() {
  Input phaseSetting{5, 6, 7, 8, 9};

  int64_t maxThrusterSignal = 0;
  do {
    int64_t signal = outputSignal(phaseSetting);
    if (signal > maxThrusterSignal)
      maxThrusterSignal = signal;
  } while (
      std::next_permutation(std::begin(phaseSetting), std::end(phaseSetting)));

  return maxThrusterSignal;
}

int64_t FeedbackLoop::outputSignal(Input const &phaseSetting) {
  amplifiers_.clear();
  for (int i = 0; i < 5; ++i) {
    Computer computer{program_};
    computer.writeInput({phaseSetting[i]});
    amplifiers_.push_back(computer);
  }

  amplifiers_[0].writeInput({0});
  int64_t current = 0;
  int64_t lastOutput = 0;
  Intcode lastCode = Intcode::Input;
  while (!(current == 0 && lastCode == Intcode::Halt)) {
    do {
      lastCode = amplifiers_[current].runInstruction();
    } while (lastCode != Intcode::Output && lastCode != Intcode::Halt);

    int64_t next = (current + 1) % 5;
    if (lastCode == Intcode::Output) {
      lastOutput = amplifiers_[current].readOutput();
      amplifiers_[next].writeInput({lastOutput});
    }

    current = next;
  }

  return lastOutput;
}