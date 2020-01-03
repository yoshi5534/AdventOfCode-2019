#include <cstdint>
#include <iostream>
#include <string>
#include <sstream>

#include <cxxopts.hpp>

#include "puzzles.h"

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
    if (result.count("help") || !result.count("puzzle"))
    {
      std::cout << options.help () << std::endl;
      return -1;
    }

    auto inputFile = result.count("input") ? result["input"].as<std::string> () : std::string();
    solve (result["puzzle"].as<int> (), inputFile);
  }
  catch (cxxopts::OptionException const& oe)
  {
    std::cout << oe.what () << std::endl;
  }

  return 0;
}