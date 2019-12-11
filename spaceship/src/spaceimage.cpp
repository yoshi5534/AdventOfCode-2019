#include <spaceimage.h>

#include <range/v3/algorithm.hpp> 
#include <range/v3/view.hpp>

using namespace AdventOfCode;
using namespace ranges;

SpaceImage::SpaceImage(int width, int height, Format data)
    : width_{width}, height_{height} {
  int layerLength = width_ * height_;
  auto layers = data | views::chunk(layerLength);
  for (auto const& layer : layers)
  {
    layers_.push_back (to <ImageLayer> (layer));
  }
}

bool SpaceImage::operator ==(SpaceImage const& other) const{
  return (layers_ == other.layers_ && width_ == other.width_ && height_ == other.height_);
}

int SpaceImage::checksum ()
{
  int minZeroLayer = 0;
  int minZeros = INT_MAX;
  for (int i = 0; i < layers_.size (); ++i)
  {
    auto zeros = ranges::count (layers_[i], 0);
    if (zeros < minZeros)
    {
      minZeros = zeros;
      minZeroLayer = i;
    }
  }
  return ranges::count (layers_[minZeroLayer], 1) * ranges::count (layers_[minZeroLayer], 2);
}

SpaceImage SpaceImage::fromDigitalSendingNetwork (int width, int height,std::string const& stream)
{
  Format imageData;
  for (char c: stream)
  {
    imageData.push_back(c -'0');
  }

  return SpaceImage(width, height, imageData);
}
