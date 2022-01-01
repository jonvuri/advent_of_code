#include <string>
#include <sstream>

#include <doctest/doctest.h>

#include "solver_01_part1.h"


unsigned long Solver_01_part1::solve(std::istream &is)
{
  long previousValue, currentValue;
  unsigned long increases = 0;

  if (!(is >> previousValue)) {
    throw "Empty or invalid input";
  }

  while (is >> currentValue) {
    if (currentValue > previousValue) {
      increases += 1;
    }
    previousValue = currentValue;
  }

  return increases;
}

TEST_CASE("testing solver for day 1 part 1 - number of value increases")
{
  Solver_01_part1 solver;
  std::istringstream is(std::string{ R"(
    199
    200
    208
    210
    200
    207
    240
    269
    260
    263
  )" });

  // Should return the number of times that a value was larger than the previous value
  CHECK(solver.solve(is) == 7);
}
