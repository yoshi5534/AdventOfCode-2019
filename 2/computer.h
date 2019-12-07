#pragma once
#include <vector>

namespace AdventOfCode
{
class Computer
{
public:
using Program = std::vector<int>;

static Program calculate (Program const& input);
};
}