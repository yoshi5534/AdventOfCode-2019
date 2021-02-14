from conans import ConanFile, CMake

class AdventOfCode2019(ConanFile):
  settings = "compiler", "build_type"
  requires = "doctest/2.3.4@bincrafters/stable", "cxxopts/2.2.1", "range-v3/0.10.0@ericniebler/stable"
  generators = "cmake"

  def build(self):
    cmake = CMake(self)
    cmake.configure()
    cmake.build()