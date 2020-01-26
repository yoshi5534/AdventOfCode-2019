#include <springdroid.h>

#include <iostream>

using namespace AdventOfCode;

namespace {
Input fromScript(SpringScript const &script) {
  Input input;

  for (auto &&line : script) {
    for (char c : line) {
      input.push_back(c);
    }
    input.push_back('\n');
  }
  return input;
}

int writeOutput(Computer &computer) {
  while (computer.hasOutput()) {
    auto output = computer.readOutput();
    if (output < 128)
      std::cout << static_cast<char>(output);
    else
      return output;
  }
  return 0;
}
} // namespace

SpringDroid::SpringDroid(Program const &program) : program_{program} {}

int SpringDroid::walk(SpringScript const &script) {
  Computer computer{program_};

  computer.writeInput(fromScript(script));
  computer.writeInput({'W', 'A', 'L', 'K', '\n'});
  computer.calculate();

  return writeOutput(computer);
}

int SpringDroid::run(SpringScript const &script) {
  Computer computer{program_};

  computer.writeInput(fromScript(script));
  computer.writeInput({'R', 'U', 'N', '\n'});
  computer.calculate();

  return writeOutput(computer);
}