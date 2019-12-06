#include <math.h>

namespace AdventOfCode
{
int fuel_for_module (int mass)
{
  return static_cast <int> (std::floor (static_cast <double> (mass) / 3.0) - 2.0);
}
};