#include <string>
#include <sstream>

#include <doctest/doctest.h>

#include "solver_03_part2.h"

unsigned long Solver_03_part2::findByBitCriteria(std::vector<unsigned long> list, unsigned long length, bool keepHigh)
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
        if (value & (1 << position)) {
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
        unsigned long value = list[i] & (1 << position);

        if (keepHigh) {
          if (tally < 0) {
            if (value) {
              keep.at(i) = false;
            }
          } else {
            if (!value) {
              keep.at(i) = false;
            }
          }
        } else {
          if (tally >= 0) {
            if (value) {
              keep.at(i) = false;
            }
          } else {
            if (!value) {
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
      throw "No result found";
    }
    if (keep[i]) {
      result = list[i];
      break;
    }
  }

  return result;
}


unsigned long Solver_03_part2::oxygenRating(std::vector<unsigned long> list, unsigned long length)
{
  return findByBitCriteria(list, length, true);
}

TEST_CASE("testing oxygen rating")
{
  Solver_03_part2 solver;
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

  CHECK(solver.oxygenRating(test_list, 5) == 23);
}


unsigned long Solver_03_part2::CO2scrubberRating(std::vector<unsigned long> const &list, unsigned long length)
{
  return findByBitCriteria(list, length, false);
}

TEST_CASE("testing CO2 scrubber rating")
{
  Solver_03_part2 solver;
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

  CHECK(solver.CO2scrubberRating(test_list, 5) == 10);
}


unsigned long Solver_03_part2::solve(std::istream &is)
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

  const auto oxygen = oxygenRating(list, length);
  const auto co2 = CO2scrubberRating(list, length);

  return oxygen * co2;
}

TEST_CASE("testing 03 part 2 solver")
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

  CHECK(solver.solve(is) == 230);
}
