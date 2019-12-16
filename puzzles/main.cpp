#include <cstdint>
#include <iostream>
#include <string>
#include <sstream>

#include <cxxopts.hpp>

int main (int argc, char** argv)
{
  cxxopts::Options options("puzzles", "Solve all puzzles of AoC 2019");
  options.add_options()
    ("p,puzzle", "Which puzzle should be solved?", cxxopts::value<int>())
    ("i,input", "Alternative input file", cxxopts::value<std::string>())
    ("h,help", "Print this help message")
    ;


  try {
    auto result = options.parse (argc, argv);
    if (result.count("help"))
      std::cout << options.help () << std::endl;
  }
  catch (cxxopts::OptionException const& oe)
  {
    std::cout << oe.what () << std::endl;
  }

  return 0;
}