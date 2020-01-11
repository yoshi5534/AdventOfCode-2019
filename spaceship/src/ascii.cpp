#include <ascii.h>

#include <droid.h> // MapPosition

#include <algorithm>
#include <iostream>
#include <map>
#include <numeric>
#include <vector>

using namespace AdventOfCode;

namespace {

using PixelCoordinate = MapPosition;
using Pixels = std::vector<PixelCoordinate>;
using CameraImage = std::map<PixelCoordinate, char>;

void print(CameraImage const &image) {
  std::for_each(std::begin(image), std::end(image),
                [&](auto const &pixel) { std::cout << pixel.second; });
}

bool isScaffold(CameraImage const &image, PixelCoordinate const &coordinate) {
  auto const pixel = image.find(coordinate);
  if (pixel != std::end(image))
    return image.at(coordinate) == '#';
  else
    return false;
}

CameraImage getImage(Program const &program) {
  CameraImage image;

  Computer computer(program);
  computer.calculate();
  int x = 0, y = 0;
  while (computer.hasOutput()) {
    char pixel = static_cast<char>(computer.readOutput());
    image[{x, y}] = pixel;
    x++;
    if (pixel == '\n') {
      x = 0;
      y++;
    }
  }
  return image;
}
} // namespace

ASCII::ASCII(Program const &program) : program_{program} {}

int ASCII::findIntersections() {
  auto image = getImage(program_);
  print(image);

  Pixels crossing;
  Pixels const neighbours{{0, 0}, {-1, 0}, {0, -1}, {1, 0}, {0, 1}};
  std::for_each(std::begin(image), std::end(image), [&](auto const &pixel) {
    if (std::all_of(std::begin(neighbours), std::end(neighbours),
                    [&](auto const &neighbour) {
                      return isScaffold(image, pixel.first + neighbour);
                    })) {
      image[pixel.first] = 'O';
      crossing.push_back(pixel.first);
    }
  });

  print(image);
  auto sum = std::accumulate(std::begin(crossing), std::end(crossing), 0,
                             [](auto const &init, auto const &position) {
                               return init + position.x * position.y;
                             });

  return sum;
}