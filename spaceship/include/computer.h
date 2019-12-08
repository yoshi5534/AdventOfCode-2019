#pragma once
#include <vector>

namespace AdventOfCode
{
using Program = std::vector<int>;
using Memory = std::vector<int>;
using Input = std::vector<int>;

class Computer
{
public:
void writeMemory (Memory const& memory);
Memory readMemory () const;

void calculate (Program const& memory);

friend Computer& operator<< (Computer& computer, Input const& input)
{
  computer.input_.insert (std::end (computer.input_), std::begin (input), std::end (input));
  return computer;
}

private:
Memory memory_;
Input input_;
};

}