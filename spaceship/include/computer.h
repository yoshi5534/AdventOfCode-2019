#pragma once
#include <deque>
#include <vector>

namespace AdventOfCode {
using Program = std::vector<int>;
using Memory = std::vector<int>;
using Input = std::deque<int>;
using Output = Memory;

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
  int relativeBase () const;
  
  void calculate();
  Intcode runInstruction ();

  void writeInput(Input const &input);
  int readInput();

  void writeOutput(Output const &output);
  int readOutput();

  void moveInstructionPointer(int offset);
  void setInstructionPointer(int address);
  int getInstructionPointer() const;

private:
  Memory memory_;
  Input input_;
  Output output_;
  int instructionPointer_;
  int relativeBase_;
};

} // namespace AdventOfCode