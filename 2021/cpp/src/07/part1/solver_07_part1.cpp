#include <climits>
#include <functional>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

#include <doctest/doctest.h>

#include "solver_07_part1.h"


std::string Solver_07_part1::solve(std::istream &is)
{
  std::vector<unsigned long> positions;

  unsigned long input = 0;
  while (is >> input) {
    positions.push_back(input);
    is.ignore(1);
  }

  unsigned long min_total_distance = ULONG_MAX;
  for (const unsigned long &pivot : positions) {
    unsigned long current_total_distance = std::transform_reduce(
      positions.begin(), positions.end(), 0U, std::plus<>(), [&](const unsigned long &p) { return p > pivot ? p - pivot : pivot - p; });

    if (current_total_distance < min_total_distance) {
      min_total_distance = current_total_distance;
    }
  }

  return std::to_string(min_total_distance);
}

TEST_CASE("testing solver for day 7 part 1 - crab submarines, constant fuel spend")
{
  Solver_07_part1 solver;
  std::istringstream is(std::string{ R"(
    16,1,2,0,4,2,7,1,2,14
  )" });

  CHECK(solver.solve(is) == "37");
}
