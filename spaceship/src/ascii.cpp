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

bool isScaffold(CameraImage const &image, PixelCoordinate const &coordinate) {
  auto const pixel = image.find(coordinate);
  if (pixel != std::end(image))
    return image.at(coordinate) == '#';
  else
    return false;
}

auto findStart(CameraImage const &image) {
  auto startPos =
      std::find_if(std::begin(image), std::end(image),
                   [](auto const &pixel) { return pixel.second == '^'; });
  return startPos->first;
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

Input fromString(std::string const &text) {
  Input input;
  input.resize(text.size());
  std::transform(std::begin(text), std::end(text), std::begin(input),
                 [](auto const &character) {
                   return static_cast<Input::value_type>(character);
                 });
  input.push_back('\n');
  return input;
}

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

PixelCoordinate reverse(PixelCoordinate const &dir) {
  if (dir == PixelCoordinate{0, 1})
    return {0, -1};
  if (dir == PixelCoordinate{1, 0})
    return {-1, 0};
  if (dir == PixelCoordinate{0, -1})
    return {0, 1};
  if (dir == PixelCoordinate{-1, 0})
    return {1, 0};

  return dir;
}

void printPath(Path const &path) {
  std::for_each(std::begin(path), std::end(path), [&](auto const &point) {
    (point < '0' || point > 'A') ? std::cout << point
                                 : std::cout << point - '0';
  });
  std::cout << std::endl;
}

Path add(Path const &old, Direction const &direction, int count) {
  auto path{old};

  if (path.size() > 0)
    path.push_back(',');
  path.push_back(direction.turn);
  path.push_back(',');
  path.push_back(count + '0');

  return path;
}

Path removeLast(Path const &old) {
  return {std::begin(old), std::next(std::begin(old), old.size() - 4)};
}

bool explore(std::vector<Path> &allPaths, CameraImage const &image,
             Pixels const &crossings,
             std::vector<std::pair<PixelCoordinate, PixelCoordinate>> visited,
             PixelCoordinate const &start, Direction const &dir, int count,
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
      nextPath = add(nextPath, currentDir, count);
      count = 0;
      currentDir = {left(currentDir.offset), 'L'};
    } else if (isScaffold(image, current + right(currentDir.offset))) {
      nextPath = add(nextPath, currentDir, count);
      count = 0;
      currentDir = {right(currentDir.offset), 'R'};
    } else {
      count++;
      nextPath = add(nextPath, currentDir, count);
      allPaths.push_back(nextPath);
      return true;
    }
  }

  nextPath = add(nextPath, currentDir, count);

  nextVisited.push_back({start, dir.offset});
  nextVisited.push_back({start, reverse(dir.offset)});

  if (std::end(nextVisited) ==
      std::find(std::begin(nextVisited), std::end(nextVisited),
                std::make_pair(current, left(currentDir.offset))))
    explore(allPaths, image, crossings, nextVisited, current,
            {left(currentDir.offset), 'L'}, 1, nextPath);

  if (std::end(nextVisited) ==
      std::find(std::begin(nextVisited), std::end(nextVisited),
                std::make_pair(current, right(currentDir.offset))))
    explore(allPaths, image, crossings, nextVisited, current,
            {right(currentDir.offset), 'R'}, 1, nextPath);

  if (std::end(nextVisited) ==
      std::find(std::begin(nextVisited), std::end(nextVisited),
                std::make_pair(current, currentDir.offset))) {
    nextPath = removeLast(nextPath);
    explore(allPaths, image, crossings, nextVisited, current, currentDir,
            count + 1, nextPath);
  }

  return false;
}

bool checkPath(CameraImage image, PixelCoordinate position, Path const &path) {
  Direction dir{{0, -1}, 'L'};
  for (int i = 0; i < static_cast<int>(path.size()) + 1; i += 4) {
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
    return true;
  }

  return false;
}

auto findValid(CameraImage const &image, std::vector<Path> const &all) {
  std::vector<Path> valid;
  auto start = findStart(image);
  std::copy_if(std::begin(all), std::end(all), std::back_inserter(valid),
               [&](auto const &path) { return checkPath(image, start, path); });

  std::sort(std::begin(valid), std::end(valid),
            [](auto const &left, auto const &right) {
              return left.size() < right.size();
            });
  return valid;
}
} // namespace

ASCII::ASCII(Program const &program) : program_{program} {}

int ASCII::findIntersections() const {
  auto const image = getImage(program_);
  auto const crossings = getCrossings(image);
  auto const sum =
      std::accumulate(std::begin(crossings), std::end(crossings), 0,
                      [](auto const &init, auto const &position) {
                        return init + position.x * position.y;
                      });

  return sum;
}

int ASCII::searchRobots() const {
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

  computer.calculate();

  auto output = computer.readOutput();
  while (computer.hasOutput())
    output = computer.readOutput();

  return output;
}

void ASCII::shortestPath() const {
  auto image = getImage(program_);
  auto crossings = getCrossings(image);
  std::vector<Path> allPaths{};

  auto start = findStart(image);
  Direction dir{{-1, 0}, 'L'};

  explore(allPaths, image, crossings, {}, start, dir, 1, {});

  auto validPaths = findValid(image, allPaths);
  printPath(validPaths[0]);
}