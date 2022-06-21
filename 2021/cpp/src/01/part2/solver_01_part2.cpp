#include <array>
#include <sstream>
#include <string>
#include <iostream>

#include <doctest/doctest.h>

#include "solver_01_part2.h"

std::string Solver_01_part2::solve(std::istream &is)
{
  // Circular buffer traveled with inputIndex (pre-filled with first 3 inputs)
  std::array<long, 4> buffer = {};
  unsigned long inputIndex = 3;

  if (!(is >> buffer[0])) {
    throw solver_runtime_error("Empty or invalid input");
  }

  if (!(is >> buffer[1]) || !(is >> buffer[2])) {
    return "";
  }

  long lastSum = buffer[0] + buffer[1] + buffer[2];
  unsigned long increases = 0;

  while (is >> buffer.at(inputIndex)) {
    long nextSum = lastSum - buffer.at((inputIndex + 1) % 4) + buffer.at(inputIndex);

    if (nextSum > lastSum) {
      increases += 1;
    }

    lastSum = nextSum;
    inputIndex = (inputIndex + 1) % 4;
  }

  return std::to_string(increases);
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
  CHECK(solver.solve(is) == "5");
}
