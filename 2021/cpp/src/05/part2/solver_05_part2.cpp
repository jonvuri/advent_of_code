#include <algorithm>
#include <map>
#include <sstream>
#include <string>
#include <tuple>

#include <doctest/doctest.h>

#include "solver_05_part2.h"


typedef std::tuple<long, long> coordinate;

std::string Solver_05_part2::solve(std::istream &is)
{
  long x1 = 0;
  long y1 = 0;
  long x2 = 0;
  long y2 = 0;
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
      entry = (entry != 0 ? entry : 0) + 1;
    }
  }

  const auto hotvents = std::count_if(heatmap.begin(), heatmap.end(), [](const auto &value) { return value.second > 1U; });

  if (hotvents < 0) {
    throw solver_runtime_error("Negative count");
  }

  return std::to_string(hotvents);
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

  CHECK(solver.solve(is) == "12");
}
