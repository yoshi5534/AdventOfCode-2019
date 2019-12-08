#include <iostream>
#include <cstdint>
#include <string>

#include <fuel.h>

int main (int argc, char** argv)
{
  std::int64_t fuel = 0;

  std::string module_mass;
  while (getline (std::cin, module_mass))
  {
    fuel += AdventOfCode::fuel_for_module (std::stoi (module_mass));
  }
  std::cout << "fuel: " << fuel << std::endl;

  return 0;
}