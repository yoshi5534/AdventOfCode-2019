#pragma once
#include <vector>

namespace AdventOfCode
{
using Program = std::vector<int>;
using Memory = std::vector<int>;

class Computer
{
public:
void writeMemory (Memory const& memory);
Memory readMemory () const;

void calculate (Program& memory);

private:
Memory memory_;
};
}