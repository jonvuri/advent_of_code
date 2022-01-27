#include <algorithm>
#include <map>
#include <sstream>
#include <string>
#include <tuple>

#include <doctest/doctest.h>

#include "solver_05_part1.h"


typedef std::tuple<unsigned long, unsigned long> coordinate;

std::string Solver_05_part1::solve(std::istream &is)
{
  unsigned long x1 = 0;
  unsigned long y1 = 0;
  unsigned long x2 = 0;
  unsigned long y2 = 0;
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
          entry += (entry != 0U ? entry : 0) + 1;
        }
      } else {
        for (unsigned long i = y1; i <= y2; i++) {
          unsigned long &entry = heatmap[std::make_tuple(x1, i)];
          entry += (entry != 0U ? entry : 0) + 1;
        }
      }
    } else if (y1 == y2) {
      if (x1 > x2) {
        for (unsigned long i = x2; i <= x1; i++) {
          unsigned long &entry = heatmap[std::make_tuple(i, y1)];
          entry += (entry != 0U ? entry : 0) + 1;
        }
      } else {
        for (unsigned long i = x1; i <= x2; i++) {
          unsigned long &entry = heatmap[std::make_tuple(i, y1)];
          entry += (entry != 0U ? entry : 0) + 1;
        }
      }
    }
  }

  const auto hotvents = std::count_if(heatmap.begin(), heatmap.end(), [](const auto &value) { return value.second > 1U; });

  if (hotvents < 0) {
    throw solver_runtime_error("Negative count");
  }

  return std::to_string(hotvents);
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

  CHECK(solver.solve(is) == "5");
}
