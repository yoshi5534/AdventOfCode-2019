#include "password.h"

#include <algorithm>
#include <array>
#include <charconv>

using namespace AdventOfCode;

namespace {
bool isSixDigit(int password) {
  if (password < 100000 || password > 999999)
    return false;

  return true;
}

bool isIncreasing(int password) {
  std::array<char, 6> digits;
  std::to_chars(digits.data(), digits.data() + digits.size(), password);

  for (int i = 0; i < digits.size() - 1; ++i) {
    if (digits[i] > digits[i + 1])
      return false;
  }

  return true;
}

bool hasTwoAdjacent(int password) {
  std::array<char, 6> digits;
  std::to_chars(digits.data(), digits.data() + digits.size(), password);

  for (char c = '0'; c <= '9'; ++c) {
    int num = std::count(digits.data(), digits.data() + digits.size(), c);
    if (num == 2)
      return true;
  }

  return false;
}
} // namespace

bool AdventOfCode::isValidPassword(int password) {
  if (!isSixDigit(password) || !isIncreasing(password) ||
      !hasTwoAdjacent(password))
    return false;

  return true;
}