#include <string>
#include <sstream>

#include <doctest/doctest.h>

#include "solver_02_part2.h"


unsigned long Solver_02_part2::solve(std::istream &is)
{
  std::string direction;
  long amount = 0;

  long horizontalPosition = 0;
  long depth = 0;
  long aim = 0;

  while ((is >> direction) && (is >> amount)) {
    if (direction == "forward") {
      horizontalPosition += amount;
      depth += amount * aim;
    } else if (direction == "down") {
      aim += amount;
    } else if (direction == "up") {
      aim -= amount;
    } else {
      throw solver_runtime_error("Invalid direction input");
    }
  }

  long result = horizontalPosition * depth;

  if (result < 0) {
    throw solver_runtime_error("Out of bounds negative result");
  }

  return static_cast<unsigned long>(result);
}

TEST_CASE("testing solver for day 2 part 2 - submarine course with aim")
{
  Solver_02_part2 solver;
  std::istringstream is(std::string{ R"(
    forward 5
    down 5
    forward 8
    up 3
    down 8
    forward 2
  )" });

  // Should return the submarine course vector (depth * horizontal position)
  CHECK(solver.solve(is) == 900);
}
