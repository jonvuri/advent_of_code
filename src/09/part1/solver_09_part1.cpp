#include <limits>
#include <string>
#include <sstream>
#include <vector>

#include <doctest/doctest.h>

#include "solver_09_part1.h"


const static size_t MAP_SIZE = 1000;


unsigned long
  Solver_09_part1::solve(std::istream &is)
{
  char heightmap[MAP_SIZE][MAP_SIZE];

  unsigned long width = 0, height;
  unsigned long x = 0, y = 0;

  // Fill up heightmap with input
  char input;
  while (is.get(input)) {
    if (input == ' ') {
      continue;
    } else if (input == '\n') {
      if (x > width) {
        width = x;
      }
      if (x != 0) {
        y += 1;
      }
      x = 0;
    } else {
      heightmap[x][y] = input;
      x += 1;
    }
  }
  height = x == 0 ? y : y + 1;

  unsigned long output_sum = 0;

  for (y = 0; y < height; ++y) {
    for (x = 0; x < width; ++x) {
      // West
      if (x > 0) {
        if (heightmap[x][y] >= heightmap[x - 1][y]) {
          continue;
        }
      }

      // North
      if (y > 0) {
        if (heightmap[x][y] >= heightmap[x][y - 1]) {
          continue;
        }
      }

      // East
      if (x < width - 1) {
        if (heightmap[x][y] >= heightmap[x + 1][y]) {
          continue;
        }
      }

      // South
      if (y < height - 1) {
        if (heightmap[x][y] >= heightmap[x][y + 1]) {
          continue;
        }
      }

      // Passed all checks, this is a low point
      if (heightmap[x][y] < '0') {
        throw "Heightmap value out of range";
      } else {
        output_sum += static_cast<unsigned long>(heightmap[x][y] - '0') + 1;
      }
    }
  }

  return output_sum;
}

TEST_CASE("testing solver for day 9 part 1 - lava vent flow")
{
  Solver_09_part1 solver;
  std::istringstream is(std::string{ R"(
    2199943210
    3987894921
    9856789892
    8767896789
    9899965678
  )" });

  CHECK(solver.solve(is) == 15);
}
