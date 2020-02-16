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
  Add(int64_t sum1, int64_t sum2, int64_t res, int64_t m)
      : summand_1{sum1}, summand_2{sum2}, result{res}, mask{m} {}
  int64_t summand_1;
  int64_t summand_2;
  int64_t result;
  int64_t mask;
};

struct Multiply : public InstructionCount<4> {
  Multiply(int64_t fac1, int64_t fac2, int64_t res, int64_t m)
      : factor_1{fac1}, factor_2{fac2}, result{res}, mask{m} {}
  int64_t factor_1;
  int64_t factor_2;
  int64_t result;
  int64_t mask;
};

struct Input : public InstructionCount<2> {
  Input(int64_t pos, int64_t m) : inputPosition{pos}, mask{m} {}
  int64_t inputPosition;
  int64_t mask;
};

struct Output : public InstructionCount<2> {
  Output(int64_t pos, int64_t m) : outputPosition{pos}, mask{m} {}
  int64_t outputPosition;
  int64_t mask;
};

struct JumpIfTrue : public InstructionCount<3> {
  JumpIfTrue(int64_t jmp, int64_t pos, int64_t m)
      : jumper{jmp}, position{pos}, mask{m} {}
  int64_t jumper;
  int64_t position;
  int64_t mask;
};

struct JumpIfFalse : public InstructionCount<3> {
  JumpIfFalse(int64_t jmp, int64_t pos, int64_t m)
      : jumper{jmp}, position{pos}, mask{m} {}
  int64_t jumper;
  int64_t position;
  int64_t mask;
};

struct LessThan : public InstructionCount<4> {
  LessThan(int64_t l, int64_t r, int64_t res, int64_t m)
      : left{l}, right{r}, result{res}, mask{m} {}
  int64_t left;
  int64_t right;
  int64_t result;
  int64_t mask;
};

struct EqualTo : public InstructionCount<4> {
  EqualTo(int64_t l, int64_t r, int64_t res, int64_t m)
      : left{l}, right{r}, result{res}, mask{m} {}
  int64_t left;
  int64_t right;
  int64_t result;
  int64_t mask;
};

struct AdjustBase : public InstructionCount<2> {
  AdjustBase(int64_t off, int64_t m) : offset{off}, mask{m} {}
  int64_t offset;
  int64_t mask;
};

struct Halt : public InstructionCount<1> {};

enum class ParameterMode { Position, Absolute, Relative };

ParameterMode getMode(int64_t parameter, int64_t mask) {
  auto bit = static_cast<int64_t>(mask / std::pow(10., parameter - 1)) %
             static_cast<int64_t>(10);
  if (bit == 0)
    return ParameterMode::Position;
  else if (bit == 1)
    return ParameterMode::Absolute;
  else if (bit == 2)
    return ParameterMode::Relative;

  return ParameterMode::Position;
}

int64_t getValue(ParameterMode mode, int64_t parameter, Computer &computer) {
  switch (mode) {
  case ParameterMode::Position:
    return computer.accessMemory()[parameter];
  case ParameterMode::Absolute:
    return parameter;
  case ParameterMode::Relative:
    return computer.accessMemory()[parameter + computer.relativeBase()];
  }

  return 0;
}

void writeValue(ParameterMode mode, int64_t parameter, Computer &computer,
                int64_t value) {
  switch (mode) {
  case ParameterMode::Position:
    computer.accessMemory()[parameter] = value;
  case ParameterMode::Absolute:
    break;
  case ParameterMode::Relative:
    computer.accessMemory()[parameter + computer.relativeBase()] = value;
  }
}

struct Interpretor {
  Computer &computer_;

  Interpretor(Computer &computer) : computer_{computer} {}

  void operator()(Add const &add) {
    auto summand_1 = getValue(getMode(1, add.mask), add.summand_1, computer_);
    auto summand_2 = getValue(getMode(2, add.mask), add.summand_2, computer_);
    writeValue(getMode(3, add.mask), add.result, computer_,
               summand_1 + summand_2);
  }

  void operator()(Multiply const &mult) {
    auto factor_1 = getValue(getMode(1, mult.mask), mult.factor_1, computer_);
    auto factor_2 = getValue(getMode(2, mult.mask), mult.factor_2, computer_);
    writeValue(getMode(3, mult.mask), mult.result, computer_,
               factor_1 * factor_2);
  }

  void operator()(Input const &in) {
    auto input = computer_.readInput();
    writeValue(getMode(1, in.mask), in.inputPosition, computer_, input);
  }

  void operator()(Output const &out) {
    auto value = getValue(getMode(1, out.mask), out.outputPosition, computer_);
    computer_.writeOutput({value});
  }

  void operator()(JumpIfTrue const &) {}

  void operator()(JumpIfFalse const &) {}

  void operator()(LessThan const &instruction) {
    auto left =
        getValue(getMode(1, instruction.mask), instruction.left, computer_);
    auto right =
        getValue(getMode(2, instruction.mask), instruction.right, computer_);

    writeValue(getMode(3, instruction.mask), instruction.result, computer_,
               (left < right) ? 1 : 0);
  }

  void operator()(EqualTo const &instruction) {
    auto left =
        getValue(getMode(1, instruction.mask), instruction.left, computer_);
    auto right =
        getValue(getMode(2, instruction.mask), instruction.right, computer_);

    writeValue(getMode(3, instruction.mask), instruction.result, computer_,
               (left == right) ? 1 : 0);
  }

  void operator()(AdjustBase const &adjustment) {
    auto offset =
        getValue(getMode(1, adjustment.mask), adjustment.offset, computer_);
    computer_.adjustBase(offset);
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

  bool jump(bool equalTrue, int64_t jumper, int64_t position, int64_t mask) {
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

Instruction getCommand(Program const &input, int64_t position) {
  int64_t opcode = input[position] % 100;
  int64_t parameterMask = input[position] / 100;
  if (opcode == to_underlying(Intcode::Add)) {
    return Add{input[position + 1], input[position + 2], input[position + 3],
               parameterMask};
  } else if (opcode == to_underlying(Intcode::Multiply)) {
    return Multiply{input[position + 1], input[position + 2],
                    input[position + 3], parameterMask};
  } else if (opcode == to_underlying(Intcode::Input)) {
    return Input{input[position + 1], parameterMask};
  } else if (opcode == to_underlying(Intcode::Output)) {
    return Output{input[position + 1], parameterMask};
  } else if (opcode == to_underlying(Intcode::JumpIfTrue)) {
    return JumpIfTrue{input[position + 1], input[position + 2], parameterMask};
  } else if (opcode == to_underlying(Intcode::JumpIfFalse)) {
    return JumpIfFalse{input[position + 1], input[position + 2], parameterMask};
  } else if (opcode == to_underlying(Intcode::LessThan)) {
    return LessThan{input[position + 1], input[position + 2],
                    input[position + 3], parameterMask};
  } else if (opcode == to_underlying(Intcode::EqualTo)) {
    return EqualTo{input[position + 1], input[position + 2],
                   input[position + 3], parameterMask};
  } else if (opcode == to_underlying(Intcode::AdjustBase)) {
    return AdjustBase{input[position + 1], parameterMask};
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
  int64_t opCode = memory_[instructionPointer_] % 100;
  auto instruction = getCommand(memory_, instructionPointer_);
  runCommand(*this, instruction);
  incrementAddress(*this, instruction);
  return to_enum<Intcode>(opCode);
}

Memory &Computer::accessMemory() { return memory_; }

int64_t Computer::relativeBase() const { return relativeBase_; }
void Computer::adjustBase(int64_t offset) { relativeBase_ += offset; }

void Computer::writeInput(Input const &input) {
  input_.insert(std::end(input_), std::begin(input), std::end(input));
}

int64_t Computer::readInput() {
  int64_t next = input_.front();
  input_.pop_front();
  return next;
}

void Computer::writeOutput(Output const &output) {
  output_.insert(std::end(output_), std::begin(output), std::end(output));
}

int64_t Computer::readOutput() {
  int64_t next = output_.front();
  output_.pop_front();
  return next;
}

void Computer::moveInstructionPointer(int64_t offset) {
  instructionPointer_ += offset;
}

void Computer::setInstructionPointer(int64_t address) {
  instructionPointer_ = address;
}

int64_t Computer::getInstructionPointer() const { return instructionPointer_; }