#include "computer.h"

#include <math.h>
#include <type_traits>
#include <variant>

using namespace AdventOfCode;

namespace {
template <typename E> constexpr auto to_underlying(E e) noexcept {
  return static_cast<std::underlying_type_t<E>>(e);
}

enum class Intcode {
  Add = 1,
  Multiply = 2,
  Input = 3,
  Output = 4,
  JumpIfTrue = 5,
  JumpIfFalse = 6,
  LessThan = 7,
  EqualTo = 8,
  Halt = 99
};

template <int T> struct InstructionCount {
  constexpr int increment() const { return T; }
};

struct Add : public InstructionCount<4> {
  int summand_1;
  int summand_2;
  int result;
  int mask;
};

struct Multiply : public InstructionCount<4> {
  int factor_1;
  int factor_2;
  int result;
  int mask;
};

struct Input : public InstructionCount<2> {
  int inputPosition;
};

struct Output : public InstructionCount<2> {
  int outputPosition;
};

struct JumpIfTrue : public InstructionCount<3> {
  int jumper;
  int position;
  int mask;
};

struct JumpIfFalse : public InstructionCount<3> {
  int jumper;
  int position;
  int mask;
};

struct Halt : public InstructionCount<1> {};

bool isPositionMode(int parameter, int mask) {
  auto bit = static_cast<int>(mask / std::pow(10., parameter - 1)) %
             static_cast<int>(std::pow(10., parameter));
  return bit == 0;
}

struct Interpretor {
  Computer &computer_;

  Interpretor(Computer &computer) : computer_{computer} {}

  void operator()(Add const &add) {
    auto memory = computer_.readMemory();
    auto summand_1 =
        isPositionMode(1, add.mask) ? memory[add.summand_1] : add.summand_1;
    auto summand_2 =
        isPositionMode(2, add.mask) ? memory[add.summand_2] : add.summand_2;
    memory[add.result] = summand_1 + summand_2;
    computer_.writeMemory(memory);
  }

  void operator()(Multiply const &mult) {
    auto memory = computer_.readMemory();
    auto factor_1 =
        isPositionMode(1, mult.mask) ? memory[mult.factor_1] : mult.factor_1;
    auto factor_2 =
        isPositionMode(2, mult.mask) ? memory[mult.factor_2] : mult.factor_2;
    memory[mult.result] = factor_1 * factor_2;
    computer_.writeMemory(memory);
  }

  void operator()(Input const &in) {
    auto memory = computer_.readMemory();
    auto input = computer_.readInput();
    memory[in.inputPosition] = input;
    computer_.writeMemory(memory);
  }

  void operator()(Output const &out) {
    auto memory = computer_.readMemory();
    computer_.writeOutput({memory[out.outputPosition]});
  }

  void operator()(JumpIfTrue const &) {}

  void operator()(JumpIfFalse const &) {}

  void operator()(Halt const &) {}
};

struct InstructionIncrement {
  Computer &computer_;
  InstructionIncrement(Computer &computer) : computer_{computer} {}

  void operator()(Add const &add) {
    computer_.moveInstructionPointer(add.increment());
  }

  void operator()(Multiply const &mult) {
    computer_.moveInstructionPointer(mult.increment());
  }

  void operator()(Input const &in) {
    computer_.moveInstructionPointer(in.increment());
  }

  void operator()(Output const &out) {
    computer_.moveInstructionPointer(out.increment());
  }

  bool jump(bool equalTrue, int jumper, int position, int mask) {
    auto memory = computer_.readMemory();
    bool jumpValue = (0 != isPositionMode(1, mask) ? memory[jumper] : jumper);
    if (jumpValue == equalTrue) {
      auto address = isPositionMode(2, mask) ? memory[position] : position;
      computer_.setInstructionPointer(address);
      return true;
    }

    return false;
  }

  void operator()(JumpIfTrue const &instruction) {
    if (!jump(true, instruction.jumper, instruction.position, instruction.mask))
      computer_.moveInstructionPointer(instruction.increment());
  }

  void operator()(JumpIfFalse const &instruction) {
    if (!jump(false, instruction.jumper, instruction.position,
              instruction.mask))
      computer_.moveInstructionPointer(instruction.increment());
  }

  void operator()(Halt const &halt) {
    computer_.moveInstructionPointer(halt.increment());
  }
};

using Instruction =
    std::variant<Add, Multiply, Input, Output, JumpIfTrue, JumpIfFalse, Halt>;

Instruction getCommand(Program const &input, int position) {
  int opcode = input[position] % 100;
  int parameterMask = std::floor(input[position] / 100);
  if (opcode == to_underlying(Intcode::Add)) {
    return Add{.summand_1 = input[position + 1],
               .summand_2 = input[position + 2],
               .result = input[position + 3],
               .mask = parameterMask};
  } else if (opcode == to_underlying(Intcode::Multiply)) {
    return Multiply{.factor_1 = input[position + 1],
                    .factor_2 = input[position + 2],
                    .result = input[position + 3],
                    .mask = parameterMask};
  } else if (opcode == to_underlying(Intcode::Input)) {
    return Input{.inputPosition = input[position + 1]};
  } else if (opcode == to_underlying(Intcode::Output)) {
    return Output{.outputPosition = input[position + 1]};
  } else if (opcode == to_underlying(Intcode::JumpIfTrue)) {
    return JumpIfTrue{.jumper = input[position + 1],
                      .position = input[position + 2],
                      .mask = parameterMask};
  } else if (opcode == to_underlying(Intcode::JumpIfFalse)) {
    return JumpIfFalse{.jumper = input[position + 1],
                       .position = input[position + 2],
                       .mask = parameterMask};
  } else if (opcode == to_underlying(Intcode::Halt)) {
    return Halt{};
  }

  return Halt{};
}

void runCommand(Computer &computer, Instruction const &instruction) {
  std::visit(Interpretor{computer}, instruction);
}

void incrementAddress(Computer &computer, Instruction const &instruction) {
  std::visit(InstructionIncrement{computer}, instruction);
}
} // namespace

void Computer::calculate(Program const &input) {
  writeMemory(input);
  instructionPointer_ = 0;

  auto instruction = getCommand(memory_, instructionPointer_);
  while (!std::holds_alternative<Halt>(instruction)) {
    runCommand(*this, instruction);
    incrementAddress(*this, instruction);
    instruction = getCommand(memory_, instructionPointer_);
  }
}

void Computer::writeMemory(Memory const &memory) { memory_ = memory; }

Memory Computer::readMemory() const { return memory_; }

void Computer::writeInput(Input const &input) {
  input_.insert(std::end(input_), std::begin(input), std::end(input));
}

int Computer::readInput() { return *input_.erase(input_.begin()); }

void Computer::writeOutput(Output const &output) {
  output_.insert(std::end(output_), std::begin(output), std::end(output));
}

int Computer::readOutput() { return *output_.erase(output_.begin()); }

void Computer::moveInstructionPointer(int offset) {
  instructionPointer_ += offset;
}

void Computer::setInstructionPointer(int address) {
  instructionPointer_ = address;
}

int Computer::getInstructionPointer() const { return instructionPointer_; }