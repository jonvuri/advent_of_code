#include <string>
#include <sstream>

#include <doctest/doctest.h>

#include "three1_solver.h"

unsigned long Three1Solver::gammaRate(std::vector<unsigned long> const &list, unsigned long length)
{
  int tallies[16] = {};

  for (unsigned long value : list) {
    for (unsigned long i = 0; i < length; i++) {
      if (value & (1 << i)) {
        tallies[i]++;
      } else {
        tallies[i]--;
      }
    }
  }

  unsigned long result = 0;

  for (unsigned long i = 0; i < length; i++) {
    if (tallies[i] > 0) {
      result += 1 << i;
    }
  }

  return result;
}

TEST_CASE("testing gamma rate")
{
  Three1Solver solver;
  std::vector<unsigned long> test_list = {
    0b00100,
    0b11110,
    0b10110,
    0b10111,
    0b10101,
    0b01111,
    0b00111,
    0b11100,
    0b10000,
    0b11001,
    0b00010,
    0b01010
  };

  CHECK(solver.gammaRate(test_list, 5) == 22);
}

unsigned long Three1Solver::epsilonRate(std::vector<unsigned long> const &list, unsigned long length)
{
  int tallies[16] = {};

  for (unsigned long value : list) {
    for (unsigned long i = 0; i < length; i++) {
      if (value & (1 << i)) {
        tallies[i]++;
      } else {
        tallies[i]--;
      }
    }
  }

  unsigned long result = 0;

  for (unsigned long i = 0; i < length; i++) {
    if (tallies[i] <= 0) {
      result += 1 << i;
    }
  }

  return result;
}

unsigned long Three1Solver::solve(std::istream &is)
{
  std::vector<unsigned long> list;

  unsigned long length = 0;
  std::string str_value;

  while (is >> str_value) {
    if (length == 0) length = str_value.length();

    unsigned long value = std::stoul(str_value, 0, 2);
    list.push_back(value);
  }

  if (length == 0) {
    throw "No nonempty lines";
  }

  const auto gamma = gammaRate(list, length);
  const auto epsilon = epsilonRate(list, length);

  return gamma * epsilon;
}

TEST_CASE("testing three1 solver")
{
  Three1Solver solver;
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
