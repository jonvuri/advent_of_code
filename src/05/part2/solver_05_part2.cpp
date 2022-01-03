#include <cmath>
#include <map>
#include <string>
#include <sstream>
#include <vector>

#include <doctest/doctest.h>

#include "solver_05_part2.h"


typedef std::tuple<long, long> coordinate;

unsigned long
  Solver_05_part2::solve(std::istream &is)
{
  long x1, y1, x2, y2;
  std::map<coordinate, long> heatmap;

  // Input format '2,2 -> 2,1'
  while (is >> x1) {
    is.ignore(1);
    is >> y1;
    is.ignore(3);
    is >> x2;
    is.ignore(1);
    is >> y2;

    // Arbitrarily choose x1, y1 as starting point to 'draw' from

    // Is x increasing, decreasing or staying the same towards x2?
    const long xDelta = x2 - x1;
    const long xIncrement = xDelta > 0 ? 1 : xDelta < 0 ? -1
                                                        : 0;

    // Is y increasing, decreasing or staying the same towards y2?
    const long yDelta = y2 - y1;
    const long yIncrement = yDelta > 0 ? 1 : yDelta < 0 ? -1
                                                        : 0;

    // How far to draw in number of points
    const long distance = std::max(std::abs(xDelta), std::abs(yDelta));

    for (long i = 0; i <= distance; i++) {
      long &entry = heatmap[std::make_tuple(x1 + (xIncrement * i), y1 + (yIncrement * i))];
      entry = (entry ? entry : 0) + 1;
    }
  }

  unsigned long hotAreas = 0;
  for (auto it = heatmap.begin(); it != heatmap.end(); ++it) {
    if (it->second > 1) {
      hotAreas += 1;
    }
  }

  return hotAreas;
}

TEST_CASE("testing solver for day 5 part 2 - hydrothermal vent vectors plus diagonals")
{
  Solver_05_part2 solver;
  std::istringstream is(std::string{ R"(
    0,9 -> 5,9
    8,0 -> 0,8
    9,4 -> 3,4
    2,2 -> 2,1
    7,0 -> 7,4
    6,4 -> 2,0
    0,9 -> 2,9
    3,4 -> 1,4
    0,0 -> 8,8
    5,5 -> 8,2
  )" });

  CHECK(solver.solve(is) == 12);
}
