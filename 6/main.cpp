#include <cstdint>
#include <iostream>
#include <orbit.h>
#include <sstream>
#include <string>

int main(int argc, char **argv) {
  using namespace AdventOfCode;

  Map map;

  std::string entry;
  while (getline(std::cin, entry)) {
    map.addOrbit(entry);
  }

  std::cout << "Checksum: " << map.checksum() << std::endl;
  std::cout << "Minimum orbital transfer: "
            << map.minimumOrbitalTransfer("YOU", "SAN") << std::endl;
  return 0;
}