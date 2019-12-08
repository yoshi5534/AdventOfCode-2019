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
  Input = 3,
  Output = 4,
  Halt = 99
};

template <int T>
struct InstructionCount
{
  constexpr int increment () const 
  {
    return T;
  }
};

struct Add : public InstructionCount<4>
{
  int summand_1;
  int summand_2;
  int result;
};

struct Multiply: public InstructionCount<4>
{
  int factor_1;
  int factor_2;
  int result;
};

struct Input : public InstructionCount<2>
{
  int inputPosition;
};

struct Output : public InstructionCount<2>
{
  int outputPosition;
};

struct Halt: public InstructionCount<1>
{};

struct Interpretor
{
  Computer& computer_;

  Interpretor(Computer& out) : computer_ {out} {}

  void operator() (Add const& add)
  {
    auto memory =computer_.readMemory ();
    memory [add.result] = memory[add.summand_1] + memory[add.summand_2];
    computer_.writeMemory (memory);
  }

  void operator() (Multiply const& mult)
  {
    auto memory =computer_.readMemory ();
    memory [mult.result] = memory[mult.factor_1] * memory[mult.factor_2];
    computer_.writeMemory (memory);
  }

  void operator() (Input const& in)
  {
    auto memory =computer_.readMemory ();
    auto input = computer_.readInput ();
    memory[in.inputPosition] = input;
    computer_.writeMemory (memory);
  }

  void operator() (Output const& out)
  {
    auto memory = computer_.readMemory ();
    computer_.writeOutput ({memory[out.outputPosition]});
  }

  void operator() (Halt const&)
  {
  }
};

struct InstructionIncrement
{
  int& pointer_;
  InstructionIncrement (int& pointer) : pointer_ {pointer} {}

  void operator() (Add const& add)
  {
    pointer_ += add.increment ();
  }

  void operator() (Multiply const& mult)
  {
    pointer_ += mult.increment ();
  }

  void operator() (Input const& in)
  {
    pointer_ += in.increment ();
  }

  void operator() (Output const& out)
  {
    pointer_ += out.increment ();
  }

  void operator() (Halt const& halt)
  {
    pointer_ += halt.increment ();
  }
};

using Instruction = std::variant <Add, Multiply, Input, Output, Halt>;

Instruction getCommand (Program const& input, int position)
{
  if (input [position] == to_underlying (Intcode::Add))
  {
    return Add {.summand_1 = input[position + 1], 
                .summand_2 = input[position + 2], 
                .result =input[position + 3]
          };
  }
  else if (input [position] == to_underlying (Intcode::Multiply))
  {
    return Multiply { .factor_1 = input[position + 1], 
                      .factor_2 = input[position + 2], 
                      .result = input[position + 3]
          };
  }
  else if (input [position] == to_underlying (Intcode::Input))
  {
    return Input {.inputPosition = input[position + 1]};
  }
  else if (input [position] == to_underlying (Intcode::Output))
  {
    return Output {.outputPosition = input[position + 1]};
  }
  else if (input [position] == to_underlying (Intcode::Halt))
  {
    return Halt {};
  }

  return Halt {};
}

void runCommand (Computer& computer, Instruction const& instruction)
{
  std::visit(Interpretor {computer}, instruction);
}

void incrementAddress (int& address, Instruction const& instruction)
{
  std::visit (InstructionIncrement {address}, instruction);
}
}

void Computer::calculate (Program const& input)
{
  writeMemory (input);

  int address = 0;
  auto instruction = getCommand (memory_, address);
  while (!std::holds_alternative<Halt> (instruction))
  {
    runCommand (*this, instruction);
    incrementAddress (address, instruction);
    instruction = getCommand (memory_, address);
  }
}

void Computer::writeMemory (Memory const& memory)
{
  memory_ = memory;
}

Memory Computer::readMemory () const
{
  return memory_;
}

void Computer::writeInput (Input const& input)
{
  input_.insert (std::end (input_), std::begin (input), std::end (input));
}

int Computer::readInput ()
{
  return *input_.erase (input_.begin ());
}

void Computer::writeOutput (Output const& output)
{
  output_.insert (std::end (output_), std::begin (output), std::end (output));
}

int Computer::readOutput ()
{
  return *output_.erase (output_.begin ());
}