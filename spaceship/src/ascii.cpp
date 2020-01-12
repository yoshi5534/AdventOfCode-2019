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
using Path = std::vector<char>;

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

Pixels getCrossings(CameraImage const &image) {
  Pixels crossings;
  Pixels const neighbours{{0, 0}, {-1, 0}, {0, -1}, {1, 0}, {0, 1}};
  std::for_each(std::begin(image), std::end(image), [&](auto const &pixel) {
    if (std::all_of(std::begin(neighbours), std::end(neighbours),
                    [&](auto const &neighbour) {
                      return isScaffold(image, pixel.first + neighbour);
                    }))
      crossings.push_back(pixel.first);
  });

  return crossings;
}

Input fromString (std::string const& text) {
  Input input;
  input.resize (text.size ());
  std::transform(std::begin(text), std::end(text),
                    std::begin(input),
                   [](auto const &character) { return static_cast <Input::value_type> (character); });
  input.push_back ('\n');
  return input;
}
} // namespace

ASCII::ASCII(Program const &program) : program_{program} {}

int ASCII::findIntersections() {
  auto const image = getImage(program_);
  print(image);
  auto const crossings = getCrossings(image);
  auto const sum =
      std::accumulate(std::begin(crossings), std::end(crossings), 0,
                      [](auto const &init, auto const &position) {
                        return init + position.x * position.y;
                      });

  return sum;
}

void ASCII::searchRobots() {
  Program movementProgram = program_;
  movementProgram[0] = 2;
  Computer computer{movementProgram};
  Input main{fromString("A,A,B,C,A,C,B,C,A,B")};
  Input a{fromString("L,4,L,10,L,6")};
  Input b{fromString("L,6,L,4,R,8,R,8")};
  Input c{fromString("L,6,R,8,L,10,L,8,L,8")};

  computer.writeInput(main);
  computer.writeInput(a);
  computer.writeInput(b);
  computer.writeInput(c);
  computer.writeInput({'n', '\n'});

  CameraImage image;

  computer.calculate();

  auto output = computer.readOutput ();
  while (computer.hasOutput ())
    output = computer.readOutput ();
    
  std::cout << "Output: " << output << std::endl;
  // int x = 0, y = 0;
  // while (computer.hasOutput()) {
  //   char pixel = static_cast<char>(computer.readOutput());
  //   image[{x, y}] = pixel;
  //   x++;
  //   if (pixel == '\n') {
  //     x = 0;
  //     y++;
  //   }
  // }

  // print(image);
}

namespace {
struct Direction {
  PixelCoordinate offset;
  char turn;

  constexpr bool operator==(Direction const &other) const {
    return (offset == other.offset) && (turn == other.turn);
  }
};

PixelCoordinate left(PixelCoordinate const &dir) {
  if (dir == PixelCoordinate{0, 1})
    return {1, 0};
  if (dir == PixelCoordinate{1, 0})
    return {0, -1};
  if (dir == PixelCoordinate{0, -1})
    return {-1, 0};
  if (dir == PixelCoordinate{-1, 0})
    return {0, 1};

  return dir;
}

PixelCoordinate right(PixelCoordinate const &dir) {
  if (dir == PixelCoordinate{0, 1})
    return {-1, 0};
  if (dir == PixelCoordinate{1, 0})
    return {0, 1};
  if (dir == PixelCoordinate{0, -1})
    return {1, 0};
  if (dir == PixelCoordinate{-1, 0})
    return {0, -1};

  return dir;
}
PixelCoordinate findNextCrossing(CameraImage const &image,
                                 PixelCoordinate const &start, Direction &dir,
                                 Pixels const &crossings) {
  PixelCoordinate current = start + dir.offset;
  int count = 1;
  while (std::find(std::begin(crossings), std::end(crossings), current) ==
         std::end(crossings)) {
    if (isScaffold(image, current + dir.offset)) {
      count++;
      current = current + dir.offset;
    } else if (isScaffold(image, current + left(dir.offset))) {
      // std::cout << dir.turn << "," << count << ",";
      count = 0;
      dir = {left(dir.offset), 'L'};
    } else if (isScaffold(image, current + right(dir.offset))) {
      // std::cout << dir.turn << "," << count << ",";
      count = 0;
      dir = {right(dir.offset), 'R'};
    } else {
      // dead end?
      return start;
    }
  }

  // std::cout << dir.turn << "," << count;
  return current;
}

void printPath(Path const &path) {
  std::for_each(std::begin(path), std::end(path),
                [&](auto const &point) { std::cout << point; });
  std::cout << std::endl;
}

bool explore(std::vector<Path> &allPathes, CameraImage const &image,
             Pixels const &crossings,
             std::vector<std::pair<PixelCoordinate, PixelCoordinate>> visited,
             PixelCoordinate const &start, int count, Direction const &dir,
             Path const &path) {
  Path nextPath{path};
  auto nextVisited{visited};
  PixelCoordinate current = start + dir.offset;
  Direction currentDir = dir;

  while (std::find(std::begin(crossings), std::end(crossings), current) ==
         std::end(crossings)) {
    if (isScaffold(image, current + currentDir.offset)) {
      count++;
      current = current + currentDir.offset;
    } else if (isScaffold(image, current + left(currentDir.offset))) {
      nextPath.push_back(',');
      nextPath.push_back(currentDir.turn);
      nextPath.push_back(',');
      nextPath.push_back(count + '0');
      count = 0;
      currentDir = {left(currentDir.offset), 'L'};
    } else if (isScaffold(image, current + right(currentDir.offset))) {
      nextPath.push_back(',');
      nextPath.push_back(currentDir.turn);
      nextPath.push_back(',');
      nextPath.push_back(count + '0');
      count = 0;
      currentDir = {right(currentDir.offset), 'R'};
    } else {
      // dead end?
      count++;
      nextPath.push_back(',');
      nextPath.push_back(currentDir.turn);
      nextPath.push_back(',');
      nextPath.push_back(count + '0');
      allPathes.push_back(nextPath);
      //printPath(nextPath);
      return true;
    }
  }

  nextPath.push_back(',');
  nextPath.push_back(currentDir.turn);
  nextPath.push_back(',');
  nextPath.push_back(count + '0');

  // printPath(nextPath);
  nextVisited.push_back({start, dir.offset});
    nextVisited.push_back({start, left(left(dir.offset))});

  if (std::end(nextVisited) ==
      std::find(std::begin(nextVisited), std::end(nextVisited),
                std::make_pair(current, left(currentDir.offset))))
    explore(allPathes, image, crossings, nextVisited, current, 1,
            {left(currentDir.offset), 'L'}, nextPath);

  if (std::end(nextVisited) ==
      std::find(std::begin(nextVisited), std::end(nextVisited),
                std::make_pair(current, right(currentDir.offset))))
    explore(allPathes, image, crossings, nextVisited, current, 1,
            {right(currentDir.offset), 'R'}, nextPath);

  if (std::end(nextVisited) ==
      std::find(std::begin(nextVisited), std::end(nextVisited),
                std::make_pair(current, currentDir.offset))) {

    nextPath.pop_back();
    nextPath.pop_back();
    nextPath.pop_back();
    nextPath.pop_back();

    // don't walk back
    explore(allPathes, image, crossings, nextVisited, current, count + 1,
            currentDir, nextPath);
  }

  return false;
}

void checkPath(CameraImage image, PixelCoordinate position, Path const &path) {

  Direction dir{{0, -1}, 'L'};
  for (int i = 0; i < path.size() + 1; i += 4) {
    char turn = path[i];
    int length = path[i + 2] - '0';
    if (turn == 'L')
      dir.offset = left(dir.offset);
    else if (turn == 'R')
      dir.offset = right(dir.offset);
    else
      throw;

    for (int move = 0; move < length; move++) {
      image[position] = 'T';
      position = position + dir.offset;
    }
  }

  if (std::find_if(std::begin(image), std::end(image), [](auto const &pixel) {
        return pixel.second == '#';
      }) == std::end(image)) {
    printPath(path);
    // print(image);
  }
}
} // namespace

void ASCII::possibleMovements() {
  auto image = getImage(program_);
  auto crossings = getCrossings(image);
  std::vector<Path> allPathes{};

  auto startPos =
      std::find_if(std::begin(image), std::end(image),
                   [](auto const &pixel) { return pixel.second == '^'; });
  if (startPos != std::end(image)) {
    auto start = startPos->first;
    Direction dir{{-1, 0}, 'L'};
    auto firstCrossing = findNextCrossing(image, start, dir, crossings);
    {
      Path path{'L', ',', '4', ',', 'L', ',', '6'};
      explore(allPathes, image, crossings, {}, firstCrossing, 1, {{-1, 0},
      'R'},
              path);
    }
    {
      Path path{'L', ',', '4', ',', 'L', ',', '6'};
      explore(allPathes, image, crossings, {}, firstCrossing, 1, {{1, 0},
      'L'},
              path);
    }
    {
      Path path{'L', ',', '4'};
      explore(allPathes, image, crossings, {std::make_pair(firstCrossing, PixelCoordinate{0,-1})}, firstCrossing, 7, {{0, 1}, 'L'},
              path);
    }
  }

  std::for_each(
      std::begin(allPathes), std::end(allPathes),
      [&](auto const &path) { checkPath(image, startPos->first, path); });
}