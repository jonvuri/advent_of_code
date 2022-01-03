#include <climits>
#include <string>
#include <sstream>
#include <vector>

#include <doctest/doctest.h>

#include "solver_07_part1.h"


unsigned long
  Solver_07_part1::solve(std::istream &is)
{
  std::vector<unsigned long> positions;

  unsigned long input;
  while (is >> input) {
    positions.push_back(input);
    is.ignore(1);
  }

  unsigned long min_total_distance = ULONG_MAX;
  unsigned long current_total_distance = 0;
  for (const unsigned long &pivot : positions) {
    for (const unsigned long &p : positions) {
      current_total_distance += p > pivot ? p - pivot : pivot - p;
    }

    if (current_total_distance < min_total_distance) {
      min_total_distance = current_total_distance;
    }

    current_total_distance = 0;
  }

  return min_total_distance;
}

TEST_CASE("testing solver for day 7 part 1 - crab submarines, constant fuel spend")
{
  Solver_07_part1 solver;
  std::istringstream is(std::string{ R"(
    16,1,2,0,4,2,7,1,2,14
  )" });

  CHECK(solver.solve(is) == 37);
}
