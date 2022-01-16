#include <algorithm>
#include <boost/functional/hash.hpp>
#include <limits>
#include <string>
#include <sstream>
#include <unordered_map>
#include <vector>

#include <doctest/doctest.h>

#include "solver_13_part1.h"

namespace {

using coord = std::pair<unsigned int, unsigned int>;

}// namespace


std::string Solver_13_part1::solve(std::istream &is)
{
  std::unordered_map<coord, bool, boost::hash<coord>> point_map_alpha;
  std::unordered_map<coord, bool, boost::hash<coord>> point_map_beta;

  // Fold boundary during fold operation, right / bottom boundaries any other time
  unsigned int x_fold = 0;
  unsigned int y_fold = 0;

  {
    unsigned int coord_x = 0;
    unsigned int coord_y = 0;
    while (is >> std::ws && is.peek() != 'f' && is >> coord_x && is.ignore(1) && is >> coord_y) {
      point_map_alpha.emplace(std::make_pair(coord_x, coord_y), true);

      if (coord_x + 1 > x_fold) {
        x_fold = coord_x + 1;
      }

      if (coord_y + 1 > y_fold) {
        y_fold = coord_y + 1;
      }
    }
  }

  // Ingest and perform fold commands
  {
    std::string command;
    is >> std::ws;
    std::getline(is, command, '=');

    const auto &source_point_map = point_map_alpha;
    auto &dest_point_map = point_map_beta;

    if (command == "fold along x") {
      is >> x_fold;

      for (const auto &point : source_point_map) {
        if (point.first.first > x_fold * 2 || point.first.second > y_fold) {
          // Point past fold boundaries (not including current fold operation)
          continue;
        }

        if (point.first.first > x_fold) {
          dest_point_map.emplace(std::make_pair(x_fold * 2 - point.first.first, point.first.second), true);
        } else {
          dest_point_map.emplace(std::make_pair(point.first.first, point.first.second), true);
        }
      }
    } else if (command == "fold along y") {
      is >> y_fold;

      for (const auto &point : source_point_map) {
        if (point.first.first > x_fold || point.first.second > y_fold * 2) {
          // Point past fold boundaries (not including current fold operation)
          continue;
        }

        if (point.first.second > y_fold) {
          dest_point_map.emplace(std::make_pair(point.first.first, y_fold * 2 - point.first.second), true);
        } else {
          dest_point_map.emplace(std::make_pair(point.first.first, point.first.second), true);
        }
      }
    } else {
      throw solver_runtime_error("Could not process fold command: " + command);
    }
  }


  const auto &point_map = point_map_beta;

  unsigned long result = 0;

  for (unsigned int y = 0; y <= y_fold; ++y) {
    for (unsigned int x = 0; x <= x_fold; ++x) {
      if (point_map.find(std::make_pair(x, y)) != point_map.end()) {
        result += 1;
      }
    }
  }

  return std::to_string(result);
}

TEST_CASE("testing solver for day 13 part 1 - dotted paper folding, just first fold")
{
  Solver_13_part1 solver;

  std::istringstream is(std::string{ R"(
      6,10
      0,14
      9,10
      0,3
      10,4
      4,11
      6,0
      6,12
      4,1
      0,13
      10,12
      3,4
      3,0
      8,4
      1,10
      2,14
      8,10
      9,0

      fold along y=7
      fold along x=5
    )" });

  CHECK(solver.solve(is) == "17");
}
