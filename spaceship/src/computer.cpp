#include "computer.h"

#include <type_traits>
#include <variant>

using namespace AdventOfCode;

namespace
{
template <typename E>
constexpr auto to_underlying(E e) noexcept
{
    return static_cast<std::underlying_type_t<E>>(e);
}

enum class Intcode
{
  Add = 1,
  Multiply = 2,
  Halt = 99
};

struct Add
{
  int summand_1;
  int summand_2;
  int result;
};

struct Multiply
{
  int factor_1;
  int factor_2;
  int result;
};

struct Halt
{};

struct Interpretor
{
  Program& out_;

  Interpretor(Program& out) : out_ {out} {}

  void operator() (Add const& add)
  {
    out_ [add.result] = out_[add.summand_1] + out_[add.summand_2];
  }

  void operator() (Multiply const& mult)
  {
    out_ [mult.result] = out_[mult.factor_1] * out_[mult.factor_2];
  }

  void operator() (Halt const&)
  {
  }

  void operator() (std::monostate)
  {
  }
};

using Instruction = std::variant <std::monostate, Add, Multiply, Halt>;

Instruction getCommand (Program const& input, int position)
{
  if (input [position] == to_underlying (Intcode::Add))
  {
    return Add {input[position + 1], input[position + 2], input[position + 3]};
  }
  else if (input [position] == to_underlying (Intcode::Multiply))
  {
    return Multiply {input[position + 1], input[position + 2], input[position + 3]};
  }
  else if (input [position] == to_underlying (Intcode::Halt))
  {
    return Halt {};
  }

  return std::monostate ();
}

Program& operator<< (Program& program, Instruction const& instruction)
{
  std::visit(Interpretor {program}, instruction);
  return program;
}
}

void Computer::calculate (Program& input)
{
  writeMemory (input);

  int address = 0;
  auto instruction = getCommand (memory_, address);
  while (!std::holds_alternative<Halt> (instruction))
  {
    memory_ << instruction;
    address+= 4;
    instruction = getCommand (memory_, address);
  }

  input = readMemory ();
}

void Computer::writeMemory (Memory const& memory)
{
  memory_ = memory;
}

Memory Computer::readMemory () const
{
  return memory_;
}