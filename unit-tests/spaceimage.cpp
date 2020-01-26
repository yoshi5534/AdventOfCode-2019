#include "doctest.h"

#include <spaceimage.h>
using namespace AdventOfCode;

TEST_CASE("two layers") {
  Format data{1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2};
  SpaceImage reference(3, 2, data);

  SpaceImage image =
      SpaceImage::fromDigitalSendingNetwork(3, 2, "123456789012");

  REQUIRE(image == reference);
}

TEST_CASE("Image") {
  SpaceImage image =
      SpaceImage::fromDigitalSendingNetwork(2, 2, "0222112222120000");
}