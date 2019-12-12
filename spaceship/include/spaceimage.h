#pragma once

#include <string>
#include <vector>

namespace AdventOfCode {
using Format = std::vector<int>;
using ImageLayer = std::vector<int>;

class SpaceImage {
public:
  SpaceImage(int width, int height, Format data);

  ImageLayer finalImage () const;
  
  bool operator==(SpaceImage const &other) const;

  static SpaceImage fromDigitalSendingNetwork(int width, int height,
                                              std::string const &stream);

  int checksum();

private:
  std::vector<ImageLayer> layers_;
  int width_;
  int height_;
};

} // namespace AdventOfCode