#include "password.h"

#include <algorithm>
#include <array>

using namespace AdventOfCode;

namespace {
bool isSixDigit(int password) {
  if (password < 100000 || password > 999999)
    return false;

  return true;
}

bool isIncreasing(int password) {
  std::array<char, 6> digits;
  auto str = std::to_string(password);
  std::copy(std::begin(str), std::next(std::begin (str), digits.size()), std::begin(digits));

  for (int i = 0; i < static_cast<int>(digits.size()) - 1; ++i) {
    if (digits[i] > digits[i + 1])
      return false;
  }

  return true;
}

bool hasTwoAdjacent(int password) {
  std::array<char, 6> digits;
  auto str = std::to_string(password);
  std::copy(std::begin(str), std::next(std::begin (str), digits.size()), std::begin(digits));

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