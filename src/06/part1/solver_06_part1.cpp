#include <array>
#include <string>
#include <sstream>
#include <vector>

#include <doctest/doctest.h>

#include "solver_06_part1.h"


static const unsigned long GENERATIONS = 9;
static const unsigned long DAYS = 80;


unsigned long
  Solver_06_part1::solve(std::istream &is)
{
  std::array<unsigned long, GENERATIONS> generations = {};// Days until spawn: 0, 1, 2, 3, 4, 5, 6, 7, 8

  unsigned long input = 0;
  while (is >> input) {
    if (input >= GENERATIONS) {
      throw solver_runtime_error("Input out of range");
    }

    generations.at(input)++;
    is.ignore(1);
  }

  for (unsigned long d = 0; d < DAYS; d++) {
    // Store number of fish now at zero
    unsigned long zero = generations[0];

    // Move all generations one day to the left
    for (unsigned long g = 0; g < GENERATIONS - 1U; g++) {
      generations.at(g) = generations.at(g + 1U);
    }

    // Spawn new fish at gen 8
    generations[GENERATIONS - 1U] = zero;

    // Move fish from gen 0 back to gen 6
    generations[GENERATIONS - 3U] += zero;
  }

  unsigned long population = 0;
  for (unsigned long i = 0; i < GENERATIONS; i++) {
    population += generations.at(i);
  }

  return population;
}

TEST_CASE("testing solver for day 6 part 1 - lanternfish growth, 80 days")
{
  Solver_06_part1 solver;
  std::istringstream is(std::string{ R"(
    3,4,3,1,2
  )" });

  CHECK(solver.solve(is) == 5934);
}
