#include <spaceimage.h>

#include <iostream>

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


ImageLayer SpaceImage::finalImage () const
{
  ImageLayer image;
  image.resize (width_ * height_);
  std::fill (std::begin(image), std::end(image), 2);

  for (auto const& layer : layers_)
  {
    for (int y = 0; y < height_; ++y)
    {
      for (int x = 0; x < width_; ++x)
      {
        if (image[width_*y + x] == 2)
          image [width_*y + x] = layer [width_*y + x];
      }
    }
  }

  return image;
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

void SpaceImage::print () const{
  auto const image = finalImage ();
  for (int y = 0; y < height_; ++y) {
    for (int x = 0; x < width_; ++x) {
      if (image[width_ * y + x] == 1)
        std::cout << '1';
      else
        std::cout << ' ';
    }
    std::cout << '\n';
  }
}