#include <map>
#include <string>
#include <sstream>
#include <vector>

#include <doctest/doctest.h>

#include "solver_05_part1.h"


typedef std::tuple<unsigned long, unsigned long> coordinate;

unsigned long
  Solver_05_part1::solve(std::istream &is)
{
  unsigned long x1, y1, x2, y2;
  std::map<coordinate, unsigned long> heatmap;

  while (is >> x1) {
    is.ignore(1);
    is >> y1;
    is.ignore(3);
    is >> x2;
    is.ignore(1);
    is >> y2;

    if (x1 == x2) {
      if (y1 > y2) {
        for (unsigned long i = y2; i <= y1; i++) {
          unsigned long &entry = heatmap[std::make_tuple(x1, i)];
          entry += (entry ? entry : 0) + 1;
        }
      } else {
        for (unsigned long i = y1; i <= y2; i++) {
          unsigned long &entry = heatmap[std::make_tuple(x1, i)];
          entry += (entry ? entry : 0) + 1;
        }
      }
    } else if (y1 == y2) {
      if (x1 > x2) {
        for (unsigned long i = x2; i <= x1; i++) {
          unsigned long &entry = heatmap[std::make_tuple(i, y1)];
          entry += (entry ? entry : 0) + 1;
        }
      } else {
        for (unsigned long i = x1; i <= x2; i++) {
          unsigned long &entry = heatmap[std::make_tuple(i, y1)];
          entry += (entry ? entry : 0) + 1;
        }
      }
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

TEST_CASE("testing solver for day 5 part 1 - hydrothermal vent vectors")
{
  Solver_05_part1 solver;
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

  CHECK(solver.solve(is) == 5);
}
