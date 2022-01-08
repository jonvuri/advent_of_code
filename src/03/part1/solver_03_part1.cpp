#include <array>
#include <string>
#include <sstream>

#include <doctest/doctest.h>

#include "solver_03_part1.h"

static const unsigned long MAX_BIT_LENGTH = 16;

unsigned long Solver_03_part1::gammaRate(std::vector<unsigned long> const &list, unsigned long length)
{
  if (length > MAX_BIT_LENGTH) {
    throw solver_runtime_error("Length too great");
  }

  std::array<int, MAX_BIT_LENGTH> tallies = {};

  for (unsigned long value : list) {
    for (unsigned long i = 0; i < length; i++) {
      if ((value & (1U << i)) != 0U) {
        tallies.at(i)++;
      } else {
        tallies.at(i)--;
      }
    }
  }

  unsigned long result = 0;

  for (unsigned long i = 0; i < length; i++) {
    if (tallies.at(i) > 0) {
      result += 1U << i;
    }
  }

  return result;
}


unsigned long Solver_03_part1::epsilonRate(std::vector<unsigned long> const &list, unsigned long length)
{
  if (length > MAX_BIT_LENGTH) {
    throw solver_runtime_error("Length too great");
  }

  std::array<int, MAX_BIT_LENGTH> tallies = {};

  for (unsigned long value : list) {
    for (unsigned long i = 0; i < length; i++) {
      if ((value & (1U << i)) != 0U) {
        tallies.at(i)++;
      } else {
        tallies.at(i)--;
      }
    }
  }

  unsigned long result = 0;

  for (unsigned long i = 0; i < length; i++) {
    if (tallies.at(i) <= 0) {
      result += 1U << i;
    }
  }

  return result;
}


unsigned long Solver_03_part1::solve(std::istream &is)
{
  std::vector<unsigned long> list;

  unsigned long length = 0;
  std::string str_value;

  while (is >> str_value) {
    if (length == 0) {
      length = str_value.length();
    }

    unsigned long value = std::stoul(str_value, nullptr, 2);
    list.push_back(value);
  }

  if (length == 0) {
    throw solver_runtime_error("No nonempty lines");
  }

  const auto gamma = gammaRate(list, length);
  const auto epsilon = epsilonRate(list, length);

  return gamma * epsilon;
}

TEST_CASE("testing solver for day 3 part 1 - counting most common bits")
{
  Solver_03_part1 solver;
  std::istringstream is(std::string{ R"(
    00100
    11110
    10110
    10111
    10101
    01111
    00111
    11100
    10000
    11001
    00010
    01010
  )" });

  CHECK(solver.solve(is) == 198);
}
