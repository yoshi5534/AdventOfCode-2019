#pragma once

#include <computer.h>

#include <string>
#include <vector>

namespace AdventOfCode {
  using SpringScript = std::vector<std::string>;

class SpringDroid {
public:
  SpringDroid(Program const &program);
  int walk(SpringScript const& script);
  int run(SpringScript const& script);
private:
  Program program_;
};

} // namespace AdventOfCode