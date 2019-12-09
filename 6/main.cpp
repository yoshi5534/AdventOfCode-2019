#include <cstdint>
#include <iostream>
#include <string>
#include <sstream>
#include <orbit.h>

int main (int argc, char** argv)
{
  using namespace AdventOfCode;
    
  Map map;

  std::string entry;
  while (getline (std::cin, entry))
  {
    map.addOrbit (entry);
  }

  std::cout << "Checksum: " << map.checksum () << std::endl;

  return 0;
}