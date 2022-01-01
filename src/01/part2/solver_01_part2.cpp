#include <string>
#include <sstream>

#include <doctest/doctest.h>

#include "solver_01_part2.h"

unsigned long Solver_01_part2::solve(std::istream &is)
{
  // Circular buffer traveled with inputIndex (pre-filled with first 3 inputs)
  long buffer[4] = {};
  unsigned long inputIndex = 3;

  if (!(is >> buffer[0])) {
    throw "Empty or invalid input";
  }

  if (!(is >> buffer[1]) || !(is >> buffer[2])) {
    return 0;
  }

  long lastSum = buffer[0] + buffer[1] + buffer[2], nextSum;
  unsigned long increases = 0;

  while (is >> buffer[inputIndex]) {
    nextSum = lastSum - buffer[(inputIndex + 1) % 4] + buffer[inputIndex];

    if (nextSum > lastSum) {
      increases += 1;
    }

    lastSum = nextSum;
    inputIndex = (inputIndex + 1) % 4;
  }

  return increases;
}

TEST_CASE("testing solver for day 1 part 2 - number of rolling sum increases")
{
  Solver_01_part2 solver;
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

  // Should return the number of times that a rolling sum of 3 values was larger than the previous sum
  CHECK(solver.solve(is) == 5);
}
