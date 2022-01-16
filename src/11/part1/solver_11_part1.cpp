#include <array>
#include <functional>
#include <limits>
#include <string>
#include <sstream>
#include <vector>

#include <doctest/doctest.h>

#include "solver_11_part1.h"


const static size_t MAP_SIZE = 50;
const static unsigned int NUM_STEPS = 100;


std::string Solver_11_part1::solve(std::istream &is)
{
  std::array<std::array<unsigned long, MAP_SIZE>, MAP_SIZE> octopus_map = {};

  unsigned long width = 0;
  unsigned long height = 0;

  {
    unsigned long x = 0;
    unsigned long y = 0;

    // Fill up octopus map with input
    char input = 0;
    while (is.get(input)) {
      if (input == ' ') {
        continue;
      }

      if (input == '\n') {
        if (x > width) {
          width = x;
        }
        if (x != 0) {
          y += 1;
        }
        x = 0;
      } else {
        int input_num = input - '0';
        if (input_num < 0 || input_num > 9) {
          throw solver_runtime_error("Input out of range");
        }

        octopus_map.at(x).at(y) = static_cast<unsigned long>(input_num);
        x += 1;
      }
    }
    height = x == 0 ? y : y + 1;
  }

  unsigned long flashes = 0;

  const std::function<void(unsigned long, unsigned long)> flash = [&](unsigned long flash_x, unsigned long flash_y) {
    ++flashes;

    auto &flash_octopus = octopus_map.at(flash_x).at(flash_y);
    flash_octopus = 0;

    for (int x = static_cast<int>(flash_x) - 1; x <= static_cast<int>(flash_x) + 1; ++x) {
      for (int y = static_cast<int>(flash_y) - 1; y <= static_cast<int>(flash_y) + 1; ++y) {
        // Check range
        if (x < 0 || x > static_cast<int>(width) - 1 || y < 0 || y > static_cast<int>(height) - 1) {
          continue;
        }

        auto &octopus = octopus_map.at(static_cast<size_t>(x)).at(static_cast<size_t>(y));

        // 0 means this one has already flashed this step (only one allowed)
        if (octopus == 0) {
          continue;
        }

        octopus += 1;

        if (octopus > 9) {
          flash(static_cast<size_t>(x), static_cast<size_t>(y));
        }
      }
    }
  };

  for (unsigned int i = 0; i < NUM_STEPS; ++i) {
    // Step 1: Increment all by 1
    for (unsigned long y = 0; y < height; ++y) {
      for (unsigned long x = 0; x < width; ++x) {
        ++octopus_map.at(x).at(y);
      }
    }

    // Step 2: Generate flashes
    for (unsigned long y = 0; y < height; ++y) {
      for (unsigned long x = 0; x < width; ++x) {
        if (octopus_map.at(x).at(y) > 9) {
          flash(x, y);
        }
      }
    }
  }

  return std::to_string(flashes);
}

TEST_CASE("testing solver for day 11 part 1 - octopus flashing")
{
  Solver_11_part1 solver;
  std::istringstream is(std::string{ R"(
    5483143223
    2745854711
    5264556173
    6141336146
    6357385478
    4167524645
    2176841721
    6882881134
    4846848554
    5283751526
  )" });

  CHECK(solver.solve(is) == "1656");
}
