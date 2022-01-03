#include <string>
#include <sstream>
#include <vector>

#include <doctest/doctest.h>

#include "solver_06_part1.h"


const int GENERATIONS = 9;
const int DAYS = 80;


unsigned long
  Solver_06_part1::solve(std::istream &is)
{
  unsigned long generations[GENERATIONS] = {};// Days until spawn: 0, 1, 2, 3, 4, 5, 6, 7, 8

  unsigned long input;
  while (is >> input) {
    if (input < 0 || input >= GENERATIONS) {
      throw "Input out of range";
    }

    generations[input]++;
    is.ignore(1);
  }

  unsigned long zero;
  for (int d = 0; d < DAYS; d++) {
    // Store number of fish now at zero
    zero = generations[0];

    // Move all generations one day to the left
    for (int g = 0; g < GENERATIONS - 1; g++) {
      generations[g] = generations[g + 1];
    }

    // Spawn new fish at gen 8
    generations[GENERATIONS - 1] = zero;

    // Move fish from gen 0 back to gen 6
    generations[GENERATIONS - 3] += zero;
  }

  unsigned long population = 0;
  for (int i = 0; i < GENERATIONS; i++) {
    population += generations[i];
  }

  return population;
}

TEST_CASE("testing solver for day 6 part 1 - lanternfish growth")
{
  Solver_06_part1 solver;
  std::istringstream is(std::string{ R"(
    3,4,3,1,2
  )" });

  CHECK(solver.solve(is) == 5934);
}
