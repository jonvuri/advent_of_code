#include <algorithm>
#include <array>
#include <limits>
#include <string>
#include <sstream>
#include <vector>

#include <doctest/doctest.h>

#include "solver_09_part2.h"

const static size_t MAP_SIZE = 1000;


unsigned long
  Solver_09_part2::solve(std::istream &is)
{
  std::array<std::array<char, MAP_SIZE>, MAP_SIZE> heightmap = {};
  std::vector<std::pair<unsigned long, unsigned long>> low_points;

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
      heightmap.at(x).at(y) = static_cast<char>(input - '0');
      x += 1;
    }
  }
  height = x == 0 ? y : y + 1;

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
      if (heightmap.at(x).at(y) < 0) {
        throw solver_runtime_error("Heightmap value out of range");
      }

      low_points.emplace_back(x, y);
    }
  }

  std::array<std::array<bool, MAP_SIZE>, MAP_SIZE> visited = {};// No need to reset, basins do not overlap

  const std::function<unsigned long(unsigned long, unsigned long, unsigned long, unsigned long)> basin_search = [&](unsigned long fromX, unsigned long fromY, unsigned long searchX, unsigned long searchY) -> unsigned long {
    if (!visited.at(searchX).at(searchY) && heightmap.at(searchX).at(searchY) < 9) {
      visited.at(searchX).at(searchY) = true;
      return (
        1 + (searchX > 0 && !(fromX == searchX - 1 && fromY == searchY) ? basin_search(searchX, searchY, searchX - 1, searchY) : 0) +//West
        (searchY > 0 && !(fromX == searchX && fromY == searchY - 1) ? basin_search(searchX, searchY, searchX, searchY - 1) : 0) +//North
        (searchX < width - 1 && !(fromX == searchX + 1 && fromY == searchY) ? basin_search(searchX, searchY, searchX + 1, searchY) : 0) +//East
        (searchY < height - 1 && !(fromX == searchX && fromY == searchY + 1) ? basin_search(searchX, searchY, searchX, searchY + 1) : 0)//South
      );
    }

    return 0;
  };

  std::vector<unsigned long> basin_sizes;
  basin_sizes.reserve(low_points.size());
  for (const auto &[lx, ly] : low_points) {
    basin_sizes.push_back(basin_search(lx, ly, lx, ly));
  }

  if (basin_sizes.size() < 3) {
    throw solver_runtime_error("Could not find 3 low points / basins");
  }

  std::sort(basin_sizes.begin(), basin_sizes.end());

  return basin_sizes[basin_sizes.size() - 3] * basin_sizes[basin_sizes.size() - 2] * basin_sizes[basin_sizes.size() - 1];
}

TEST_CASE("testing solver for day 9 part 2 - lava vent flow with basins")
{
  Solver_09_part2 solver;
  std::istringstream is(std::string{ R"(
    2199943210
    3987894921
    9856789892
    8767896789
    9899965678
  )" });

  CHECK(solver.solve(is) == 1134);
}
