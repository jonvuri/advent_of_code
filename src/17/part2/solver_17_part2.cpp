#include <cmath>
#include <regex>
#include <set>
#include <string>
#include <sstream>
#include <vector>

#include <doctest/doctest.h>

#include "solver_17_part2.h"


namespace {

// We assume that x_min and x_max are always positive, and that y_min and y_max are always negative
std::set<std::pair<int, int>> find_all_initial_velocities(const int x_min, const int x_max, const int y_min, const int y_max)
{
  const auto hits_target_area = [&](int vx, int vy) -> bool {
    // Travel velocity from starting point until target area is hit or passed
    int x = 0;
    int y = 0;

    while (true) {
      if (x > x_max) {
        return false;// Target area has been passed
      }
      if (vx == 0) {
        if (not((x_min <= x) and (x <= x_max))) {
          return false;// "
        }
        if (y < y_min) {
          return false;// "
        }
      }

      if ((x_min <= x) and (x <= x_max) and (y_min <= y) and (y <= y_max)) {
        return true;// Target area hit
      }

      x += vx;
      y += vy;

      if (vx > 0) {
        vx -= 1;
      }
      vy -= 1;
    }
  };

  std::set<std::pair<int, int>> velocities;

  const int y_amplitude = std::max(std::abs(y_min), std::abs(y_max));
  const int min_y_velocity = -y_amplitude;
  const int max_y_velocity = y_amplitude + 1;

  // This equation is the triangular number of the initial x velocity that just reaches x_min -
  // that is, solving x_min = (x_vel * (x_vel + 1)) / 2 for x_vel
  const int min_x_velocity = static_cast<int>(std::floor((std::sqrt(8 * x_min + 1) - 1) / 2));
  const int max_x_velocity = x_max + 1;

  for (int x_velocity = min_x_velocity; x_velocity <= max_x_velocity; ++x_velocity) {
    for (int y_velocity = min_y_velocity; y_velocity <= max_y_velocity; ++y_velocity) {
      if (hits_target_area(x_velocity, y_velocity)) {
        velocities.insert(std::make_pair(x_velocity, y_velocity));
      }
    }
  }

  return velocities;
}

}// namespace


std::string Solver_17_part2::solve(std::istream &is)
{
  std::string input;
  std::getline(is >> std::ws, input);

  std::smatch match_results;
  bool match = std::regex_search(input, match_results, std::regex(R"(target area: x=([\d-]+)\.\.([\d-]+), y=([\d-]+)\.\.([\d-]+))"), std::regex_constants::match_default);
  if (!match) {
    throw solver_runtime_error("Invalid input");
  }

  int x_min = std::stoi(match_results.str(1));
  int x_max = std::stoi(match_results.str(2));
  int y_min = std::stoi(match_results.str(3));
  int y_max = std::stoi(match_results.str(4));

  const auto velocities = find_all_initial_velocities(x_min, x_max, y_min, y_max);

  return std::to_string(velocities.size());
}

TEST_CASE("testing solver for day 17 part 1 - probe launch all possible velocities")
{
  Solver_17_part2 solver;

  {
    std::istringstream is(std::string{ R"(
      target area: x=20..30, y=-10..-5
    )" });

    CHECK(solver.solve(is) == "112");
  }
}
