#include <ascii.h>

#include <computer.h>
#include <droid.h>

#include <algorithm>
#include <iostream>
#include <map>
#include <numeric>
#include <tuple>
#include <vector>

using namespace AdventOfCode;


 ASCII::ASCII(Program const &program) : computer_{program},image_{}, width_{0}, height_{0} {
    computer_.calculate();
    int x = 0, y = 0;
    while (computer_.hasOutput()) {
      char pixel = static_cast<char>(computer_.readOutput());
      image_[{x, y}] = pixel;
      x++;
      if (pixel == '\n') {
        width_ = std::max(width_, x);
        x = 0;
        y++;
      }
    }
    height_ = y;
  }

  void ASCII::print() {
    std::for_each(std::begin(image_), std::end(image_),
                  [&](auto const &pixel) { std::cout << pixel.second; });
  }

  bool ASCII::isScaffold(PixelCoordinate coordinate) {
    return image_[coordinate] == '#';
  }

  int ASCII::findIntersections() {
    Pixels crossing;
    Pixels const neighbours{{0, 0}, {-1, 0}, {0, -1}, {1, 0}, {0, 1}};
    std::for_each(std::begin(image_), std::end(image_), [&](auto const &pixel) {
      if (std::all_of(std::begin(neighbours), std::end(neighbours),
                      [&](auto const &neighbour) {
                        return isScaffold(pixel.first + neighbour);
                      })) {
        image_[pixel.first] = 'O';
        crossing.push_back(pixel.first);
      }
    });

    auto sum = std::accumulate(std::begin(crossing), std::end(crossing), 0,
                               [](auto const &init, auto const &position) {
                                 return init + position.x * position.y;
                               });

    return sum;
  }