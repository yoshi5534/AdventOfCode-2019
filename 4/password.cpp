#include "password.h"

#include <array>
#include <charconv>

using namespace AdventOfCode;

namespace
{
  bool isSixDigit (int password)
  {
    if (password < 100000 || password > 999999)
      return false;

    return true;
  }

  bool hasToAdjacent(int password)
  {
    std::array<char, 6> digits;
    std::to_chars (digits.data (), digits.data () + digits.size (), password);

    for (int i = 0; i < digits.size () - 1; ++i)
    {
      if (digits[i] == digits [i+1])
        return true;
    }

    return false;
  }

  bool isIncreasing(int password)
  {
    std::array<char, 6> digits;
    std::to_chars (digits.data (), digits.data () + digits.size (), password);

    for (int i = 0; i < digits.size () - 1; ++i)
    {
      if (digits[i] > digits [i+1])
        return false;
    }

    return true;
  }
}

bool AdventOfCode::isValidPassword (int password)
{
  if (!isSixDigit(password) || !hasToAdjacent(password) || !isIncreasing (password))
    return false;

  return true;
}