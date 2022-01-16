#include <string>
#include <sstream>

#include <doctest/doctest.h>

#include "solver_03_part2.h"

unsigned long Solver_03_part2::findByBitCriteria(const std::vector<unsigned long> &list, unsigned long length, bool keepHigh)
{
  const auto size = list.size();
  std::vector<bool> keep(size, true);
  auto position = length - 1;// Current bit position to check
  auto count = size;// Remaining count of matching values in list
  auto tally = 0;// Current tally - positive or 0 if 1 bits are more common, negative otherwise

  while (count > 1) {
    // Compute 0/1 tally for remaining set
    for (unsigned long i = 0; i < size; i++) {
      if (keep[i]) {
        unsigned long value = list[i];
        if ((value & (1U << position)) != 0U) {
          tally++;
        } else {
          tally--;
        }
      }
    }

    // Reset count to update again below
    count = 0;

    // Based on tally, reject values that don't match common bit at this bit position
    for (unsigned long i = 0; i < size; i++) {
      if (keep[i]) {
        unsigned long value = list[i] & (1U << position);

        if (keepHigh) {
          if (tally < 0) {
            if (value != 0U) {
              keep.at(i) = false;
            }
          } else {
            if (value == 0U) {
              keep.at(i) = false;
            }
          }
        } else {
          if (tally >= 0) {
            if (value != 0U) {
              keep.at(i) = false;
            }
          } else {
            if (value == 0U) {
              keep.at(i) = false;
            }
          }
        }
      }

      if (keep[i]) {
        count++;
      }
    }

    position--;
    tally = 0;
  }

  unsigned long result = 0;

  for (unsigned long i = 0; i <= size; i++) {
    if (i == size) {
      throw solver_runtime_error("No result found");
    }
    if (keep[i]) {
      result = list[i];
      break;
    }
  }

  return result;
}


unsigned long Solver_03_part2::oxygenRating(const std::vector<unsigned long> &list, unsigned long length)
{
  return findByBitCriteria(list, length, true);
}


unsigned long Solver_03_part2::CO2scrubberRating(const std::vector<unsigned long> &list, unsigned long length)
{
  return findByBitCriteria(list, length, false);
}


std::string Solver_03_part2::solve(std::istream &is)
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

  const auto oxygen = oxygenRating(list, length);
  const auto co2 = CO2scrubberRating(list, length);

  return std::to_string(oxygen * co2);
}

TEST_CASE("testing solver for day 3 part 2 - successive filtering by most common bits")
{
  Solver_03_part2 solver;
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

  CHECK(solver.solve(is) == "230");
}
