#pragma once

#include <string>
#include <vector>

namespace AdventOfCode {
using Format = std::vector<int>;
using ImageLayer = std::vector<int>;

class SpaceImage {
public:
  SpaceImage(int width, int height, Format data);
  int width() const { return width_; }
  int height() const { return height_; }

  ImageLayer finalImage() const;
  ImageLayer singleLayer(int layer) const { return layers_[layer]; }

  bool operator==(SpaceImage const &other) const;

  static SpaceImage fromDigitalSendingNetwork(int width, int height,
                                              std::string const &stream);

  int checksum();
  void print() const;

private:
  std::vector<ImageLayer> layers_;
  int width_;
  int height_;
};

} // namespace AdventOfCode