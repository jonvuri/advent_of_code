#include <algorithm>
#include <numeric>
#include <string>
#include <sstream>
#include <vector>

#include <doctest/doctest.h>

#include "solver_07_part2.h"

namespace {

unsigned long mean(const std::vector<unsigned long> &list)
{
  const auto size = list.size();

  const auto mean = std::reduce(list.begin(), list.end());

  // Safe, fast way to do positive integer division without overflows
  return (mean / size) + static_cast<unsigned long>(mean % size != 0);
}

// Formula to find 'triangular' number - sum of all numbers from 1 to n.
// e.g. 1 + 2 + 3 + 4 + 5 + 6, for n = 6
unsigned long triangular(const unsigned long &n)
{
  return n == 0 ? 0 : (n * (n + 1)) / 2;
}

unsigned long total_distance(const std::vector<unsigned long> &list, const unsigned long position)
{
  unsigned long distance = 0;
  for (const unsigned long &p : list) {
    const auto current_distance = triangular(p > position ? p - position : position - p);
    distance += current_distance;
  }
  return distance;
}

}// namespace


std::string Solver_07_part2::solve(std::istream &is)
{
  std::vector<unsigned long> positions;

  unsigned long input = 0;
  while (is >> input) {
    positions.push_back(input);
    is.ignore(1);
  }

  unsigned long mean_position = mean(positions);

  // Search candidate points (mean or mean minus 1)
  return std::to_string(std::min(total_distance(positions, mean_position - 1), total_distance(positions, mean_position)));
}

TEST_CASE("testing solver for day 7 part 2 - crab submarines, increasing fuel spend")
{
  Solver_07_part2 solver;
  std::istringstream is(std::string{ R"(
    16,1,2,0,4,2,7,1,2,14
  )" });

  CHECK(solver.solve(is) == "168");
}
