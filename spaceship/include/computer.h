#pragma once
#include <vector>

namespace AdventOfCode
{
using Program = std::vector<int>;
using Memory = std::vector<int>;
using Input = Memory;
using Output = Memory;

class Computer
{
public:
void writeMemory (Memory const& memory);
Memory readMemory () const;

void calculate (Program const& memory);

void writeInput (Input const& input);
int readInput ();

void writeOutput (Output const& output);
int readOutput ();

void moveInstructionPointer (int offset);
void setInstructionPointer (int address);
int getInstructionPointer () const;

private:
Memory memory_;
Input input_;
Output output_;
int instructionPointer_;
};

}