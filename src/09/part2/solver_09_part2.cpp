#include <algorithm>
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
  char heightmap[MAP_SIZE][MAP_SIZE];
  std::vector<std::pair<unsigned long, unsigned long>> low_points;

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
      heightmap[x][y] = input - '0';
      x += 1;
    }
  }
  height = x == 0 ? y : y + 1;

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
      if (heightmap[x][y] < 0) {
        throw "Heightmap value out of range";
      } else {
        low_points.push_back({ x, y });
      }
    }
  }

  std::vector<unsigned long> basin_sizes;
  bool visited[MAP_SIZE][MAP_SIZE]{};// No need to reset, basins do not overlap

  const std::function<unsigned long(unsigned long, unsigned long, unsigned long, unsigned long)> basin_search = [&](unsigned long fromX, unsigned long fromY, unsigned long searchX, unsigned long searchY) -> unsigned long {
    if (!visited[searchX][searchY] && heightmap[searchX][searchY] < 9) {
      visited[searchX][searchY] = true;
      return (
        1 + (searchX > 0 && !(fromX == searchX - 1 && fromY == searchY) ? basin_search(searchX, searchY, searchX - 1, searchY) : 0) +//West
        (searchY > 0 && !(fromX == searchX && fromY == searchY - 1) ? basin_search(searchX, searchY, searchX, searchY - 1) : 0) +//North
        (searchX < width - 1 && !(fromX == searchX + 1 && fromY == searchY) ? basin_search(searchX, searchY, searchX + 1, searchY) : 0) +//East
        (searchY < height - 1 && !(fromX == searchX && fromY == searchY + 1) ? basin_search(searchX, searchY, searchX, searchY + 1) : 0)//South
      );
    } else {
      return 0;
    }
  };

  for (const auto &[lx, ly] : low_points) {
    basin_sizes.push_back(basin_search(lx, ly, lx, ly));
  }

  if (basin_sizes.size() < 3) {
    throw "Could not find 3 low points / basins";
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
