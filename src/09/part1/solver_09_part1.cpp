#include <array>
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
  std::array<std::array<char, MAP_SIZE>, MAP_SIZE> heightmap = {};

  unsigned long width = 0;
  unsigned long height = 0;
  unsigned long x = 0;
  unsigned long y = 0;

  // Fill up heightmap with input
  char input = 0;
  while (is.get(input)) {
    if (input == ' ') {
      continue;
    }

    if (input == '\n') {
      if (x > width) {
        width = x;
      }
      if (x != 0) {
        y += 1;
      }
      x = 0;
    } else {
      heightmap.at(x).at(y) = input;
      x += 1;
    }
  }
  height = x == 0 ? y : y + 1;

  unsigned long output_sum = 0;

  for (y = 0; y < height; ++y) {
    for (x = 0; x < width; ++x) {
      // West
      if (x > 0) {
        if (heightmap.at(x).at(y) >= heightmap.at(x - 1).at(y)) {
          continue;
        }
      }

      // North
      if (y > 0) {
        if (heightmap.at(x).at(y) >= heightmap.at(x).at(y - 1)) {
          continue;
        }
      }

      // East
      if (x < width - 1) {
        if (heightmap.at(x).at(y) >= heightmap.at(x + 1).at(y)) {
          continue;
        }
      }

      // South
      if (y < height - 1) {
        if (heightmap.at(x).at(y) >= heightmap.at(x).at(y + 1)) {
          continue;
        }
      }

      // Passed all checks, this is a low point
      if (heightmap.at(x).at(y) < '0') {
        throw solver_runtime_error("Heightmap value out of range");
      }

      output_sum += static_cast<unsigned long>(heightmap.at(x).at(y) - '0') + 1;
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
