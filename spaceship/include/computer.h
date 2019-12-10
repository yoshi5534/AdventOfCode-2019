#pragma once
#include <deque>
#include <vector>

namespace AdventOfCode {
using Program = std::vector<int>;
using Memory = std::vector<int>;
using Input = std::deque<int>;
using Output = Memory;

class Computer {
public:
  Memory& accessMemory ();
  
  void calculate(Program const &memory);

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
};

} // namespace AdventOfCode