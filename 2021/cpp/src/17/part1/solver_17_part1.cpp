#include <cstdlib>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

#include <doctest/doctest.h>

#include "solver_17_part1.h"


namespace {

int max_y_height(int y0)
{
  int lowest_target_y = std::abs(y0);
  int starting_y_velocity = lowest_target_y - 1;

  // Triangular number, counts steps down from max height (1 + 2 + .. + n)
  int max_arc_height = (starting_y_velocity * (starting_y_velocity + 1)) / 2;

  return max_arc_height;
}

}// namespace


std::string Solver_17_part1::solve(std::istream &is)
{
  std::string input;
  std::getline(is >> std::ws, input);

  std::smatch match_results;
  bool match = std::regex_search(input, match_results, std::regex(R"(target area: x=([\d-]+)\.\.([\d-]+), y=([\d-]+)\.\.([\d-]+))"), std::regex_constants::match_default);
  if (!match) {
    throw solver_runtime_error("Invalid input");
  }

  // Out of x0 (1), x1 (2), y0 (3), and y1 (4) in the input, we only need y0 for this calculation
  int y0 = std::stoi(match_results.str(3));

  return std::to_string(max_y_height(y0));
}

TEST_CASE("testing solver for day 17 part 1 - probe launch")
{
  Solver_17_part1 solver;

  {
    std::istringstream is(std::string{ R"(
      target area: x=20..30, y=-10..-5
    )" });

    CHECK(solver.solve(is) == "45");
  }
}
