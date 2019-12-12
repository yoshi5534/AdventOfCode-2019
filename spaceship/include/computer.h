#pragma once
#include <cstdint>
#include <deque>
#include <vector>

namespace AdventOfCode {
using Program = std::vector<int64_t>;
using Memory = std::vector<int64_t>;
using Input = std::deque<int64_t>;
using Output = std::deque<int64_t>;

enum class Intcode {
  Add = 1,
  Multiply = 2,
  Input = 3,
  Output = 4,
  JumpIfTrue = 5,
  JumpIfFalse = 6,
  LessThan = 7,
  EqualTo = 8,
  AdjustBase = 9,
  Halt = 99
};

class Computer {
public:
  Computer (Program const& program);

  Memory& accessMemory ();
  int64_t relativeBase () const;
  void adjustBase (int64_t offset);
  
  void calculate();
  Intcode runInstruction ();

  void writeInput(Input const &input);
  int64_t readInput();

  void writeOutput(Output const &output);
  int64_t readOutput();

  void moveInstructionPointer(int64_t offset);
  void setInstructionPointer(int64_t address);
  int64_t getInstructionPointer() const;

private:
  Memory memory_;
  Input input_;
  Output output_;
  int64_t instructionPointer_;
  int64_t relativeBase_;
};

} // namespace AdventOfCode