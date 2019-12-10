#include <amplifier.h>

#include <algorithm>
#include <random>

using namespace AdventOfCode;

namespace {
int getSignal(Input phaseSetting, Program const &program) {
  int signal = 0;
  for (int i = 0; i < 5; ++i) {
    Computer amplifier;
    amplifier.writeInput({phaseSetting[i]});
    amplifier.writeInput({signal});
    amplifier.calculate(program);
    signal = {amplifier.readOutput()};
  }

  return signal;
}
} // namespace

int AmplifierChain::maxThrusterSignal(Program const &program) {
  Input phaseSetting{0, 1, 2, 3, 4};

  int maxThrusterSignal = 0;
  auto rng = std::default_random_engine{};
  for (auto i = 0; i < 5*4*3*2*2; ++i)
  {
    std::shuffle(std::begin(phaseSetting), std::end(phaseSetting), rng);
    int signal = getSignal (phaseSetting, program);
    if (signal > maxThrusterSignal)
      maxThrusterSignal = signal;
  }

  return maxThrusterSignal;
}