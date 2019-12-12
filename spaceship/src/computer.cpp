#include "computer.h"

#include <math.h>
#include <type_traits>
#include <variant>

using namespace AdventOfCode;

namespace {
namespace details {

template <typename E>
using enable_enum_t =
    typename std::enable_if<std::is_enum<E>::value,
                            typename std::underlying_type<E>::type>::type;

} // namespace details

template <typename E>
constexpr inline details::enable_enum_t<E> to_underlying(E e) noexcept {
  return static_cast<typename std::underlying_type<E>::type>(e);
}

template <typename E, typename T>
constexpr inline typename std::enable_if<
    std::is_enum<E>::value && std::is_integral<T>::value, E>::type
to_enum(T value) noexcept {
  return static_cast<E>(value);
}

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
  int mask;
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

struct LessThan : public InstructionCount<4> {
  int left;
  int right;
  int result;
  int mask;
};

struct EqualTo : public InstructionCount<4> {
  int left;
  int right;
  int result;
  int mask;
};

struct AdjustBase : public InstructionCount<2> {
  int offset;
};

struct Halt : public InstructionCount<1> {};

enum class ParameterMode { Position, Absolute, Relative };

ParameterMode getMode(int parameter, int mask) {
  auto bit = static_cast<int>(mask / std::pow(10., parameter - 1)) %
             static_cast<int>(std::pow(10., parameter));
  if (bit == 0)
    return ParameterMode::Position;
  else if (bit == 1)
    return ParameterMode::Absolute;
  else if (bit == 2)
    return ParameterMode::Relative;

  return ParameterMode::Position;
}

int getValue(ParameterMode mode, int parameter, Computer &computer) {
  switch (mode) {
  case ParameterMode::Position:
    return computer.accessMemory()[parameter];
  case ParameterMode::Absolute:
    return parameter;
  case ParameterMode::Relative:
    return computer.accessMemory()[parameter + computer.relativeBase()];
  }
}

struct Interpretor {
  Computer &computer_;

  Interpretor(Computer &computer) : computer_{computer} {}

  void operator()(Add const &add) {
    auto &memory = computer_.accessMemory();
    auto summand_1 = getValue(getMode(1, add.mask), add.summand_1, computer_);
    auto summand_2 = getValue(getMode(2, add.mask), add.summand_2, computer_);
    memory[add.result] = summand_1 + summand_2;
  }

  void operator()(Multiply const &mult) {
    auto &memory = computer_.accessMemory();
    auto factor_1 = getValue(getMode(1, mult.mask), mult.factor_1, computer_);
    auto factor_2 = getValue(getMode(2, mult.mask), mult.factor_2, computer_);
    memory[mult.result] = factor_1 * factor_2;
  }

  void operator()(Input const &in) {
    auto &memory = computer_.accessMemory();
    auto input = computer_.readInput();
    memory[in.inputPosition] = input;
  }

  void operator()(Output const &out) {
    auto value = getValue(getMode(1, out.mask), out.outputPosition, computer_);
    computer_.writeOutput({value});
  }

  void operator()(JumpIfTrue const &) {}

  void operator()(JumpIfFalse const &) {}

  void operator()(LessThan const &instruction) {
    auto &memory = computer_.accessMemory();
    auto left =
        getValue(getMode(1, instruction.mask), instruction.left, computer_);
    auto right =
        getValue(getMode(2, instruction.mask), instruction.right, computer_);
    if (left < right) {
      memory[instruction.result] = 1;
    } else {
      memory[instruction.result] = 0;
    }
  }

  void operator()(EqualTo const &instruction) {
    auto &memory = computer_.accessMemory();
    auto left =
        getValue(getMode(1, instruction.mask), instruction.left, computer_);
    auto right =
        getValue(getMode(2, instruction.mask), instruction.right, computer_);

    if (left == right) {
      memory[instruction.result] = 1;
    } else {
      memory[instruction.result] = 0;
    }
  }

  void operator()(AdjustBase const &adjustment) {
    computer_.adjustBase(adjustment.offset);
  }

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
    bool jumpValue = (0 != getValue(getMode(1, mask), jumper, computer_));
    if (jumpValue == equalTrue) {
      auto address = getValue(getMode(2, mask), position, computer_);
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

  void operator()(LessThan const &instruction) {
    computer_.moveInstructionPointer(instruction.increment());
  }

  void operator()(EqualTo const &instruction) {
    computer_.moveInstructionPointer(instruction.increment());
  }

  void operator()(AdjustBase const &instruction) {
    computer_.moveInstructionPointer(instruction.increment());
  }

  void operator()(Halt const &halt) {
    computer_.moveInstructionPointer(halt.increment());
  }
};

using Instruction =
    std::variant<Add, Multiply, Input, Output, JumpIfTrue, JumpIfFalse,
                 LessThan, EqualTo, AdjustBase, Halt>;

Instruction getCommand(Program const &input, int position) {
  int opcode = input[position] % 100;
  int parameterMask = input[position] / 100;
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
    return Output{.outputPosition = input[position + 1], .mask = parameterMask};
  } else if (opcode == to_underlying(Intcode::JumpIfTrue)) {
    return JumpIfTrue{.jumper = input[position + 1],
                      .position = input[position + 2],
                      .mask = parameterMask};
  } else if (opcode == to_underlying(Intcode::JumpIfFalse)) {
    return JumpIfFalse{.jumper = input[position + 1],
                       .position = input[position + 2],
                       .mask = parameterMask};
  } else if (opcode == to_underlying(Intcode::LessThan)) {
    return LessThan{.left = input[position + 1],
                    .right = input[position + 2],
                    .result = input[position + 3],
                    .mask = parameterMask};
  } else if (opcode == to_underlying(Intcode::EqualTo)) {
    return EqualTo{.left = input[position + 1],
                   .right = input[position + 2],
                   .result = input[position + 3],
                   .mask = parameterMask};
  } else if (opcode == to_underlying(Intcode::AdjustBase)) {
    return AdjustBase{.offset = input[position + 1]};
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

Computer::Computer(Program const &program)
    : memory_{program}, instructionPointer_{0}, relativeBase_{0} {
  memory_.resize(program.size() * 10);
  std::fill(std::begin(memory_) + program.size(), std::end(memory_), 0);
}

void Computer::calculate() {
  auto instruction = getCommand(memory_, instructionPointer_);
  while (!std::holds_alternative<Halt>(instruction)) {
    runCommand(*this, instruction);
    incrementAddress(*this, instruction);
    instruction = getCommand(memory_, instructionPointer_);
  }
}

Intcode Computer::runInstruction() {
  int opCode = memory_[instructionPointer_] % 100;
  auto instruction = getCommand(memory_, instructionPointer_);
  runCommand(*this, instruction);
  incrementAddress(*this, instruction);
  return to_enum<Intcode>(opCode);
}

Memory &Computer::accessMemory() { return memory_; }

int Computer::relativeBase() const { return relativeBase_; }
void Computer::adjustBase(int offset) { relativeBase_ += offset; }

void Computer::writeInput(Input const &input) {
  input_.insert(std::end(input_), std::begin(input), std::end(input));
}

int Computer::readInput() {
  int next = input_.front();
  input_.pop_front();
  return next;
}

void Computer::writeOutput(Output const &output) {
  output_.insert(std::end(output_), std::begin(output), std::end(output));
}

int Computer::readOutput() {
  int next = output_.front();
  output_.pop_front();
  return next;
}

void Computer::moveInstructionPointer(int offset) {
  instructionPointer_ += offset;
}

void Computer::setInstructionPointer(int address) {
  instructionPointer_ = address;
}

int Computer::getInstructionPointer() const { return instructionPointer_; }