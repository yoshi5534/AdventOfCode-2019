#pragma once
#include <vector>
#include <variant>

namespace AdventOfCode
{
using Program = std::vector<int>;

class Computer
{
public:

static void calculate (Program& memory);
};
}