from conans import ConanFile, CMake

class AdventOfCode2019(ConanFile):
  settings = "compiler", "build_type"
  requires = "doctest/2.3.4@bincrafters/stable", "cxxopts/2.0.0@conan-cpp/latest", "range-v3/0.10.0@ericniebler/stable"
  generators = "cmake"

  def build(self):
    cmake = CMake(self)
    cmake.configure()
    cmake.build()