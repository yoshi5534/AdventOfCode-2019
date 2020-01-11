# advent-of-code-2019
C++ Solutions for [Advent of Code 2019](https://adventofcode.com/)

# Build in Visual Studio Code Remote - Containers

Using Visual Studio Code and the [Remote Containers Extension](https://code.visualstudio.com/docs/remote/containers) open the folder in container mode.

# Build with docker

With docker installed run:

    docker build -t yoshi5534/aoc2019 .
    docker run -v /var/run/docker.sock:/var/run/docker.sock -e workspace=`pwd` yoshi5534/aoc2019

This creates a second container from the Visual Studio Code devcontainer and bind mounts the current directory. The build output is found in ./build.

# Run puzzles

From ./build/bin run:

    ./puzzles -p X

Inputs are read from ./input/X.txt