#include <algorithm>
#include <boost/functional/hash.hpp>
#include <chrono>
#include <string>
#include <sstream>
#include <unordered_map>
#include <vector>

#include <doctest/doctest.h>

#include "solver_13_part1.h"

namespace {

using coord = std::pair<unsigned int, unsigned int>;

const unsigned long ITERATIONS = 1000;

}// namespace


unsigned long Solver_13_part1::solve(std::istream &is)
{
  std::vector<coord> points;

  unsigned int coord_x = 0;
  unsigned int coord_y = 0;
  while (is >> coord_x && is.ignore(256, ',') && is >> coord_y) {
    points.emplace_back(coord_x, coord_y);
  }


  // Pair map version
  auto p_start = std::chrono::high_resolution_clock::now();

  unsigned long p_result = 0;
  unsigned long p_reflect_result = 0;

  for (unsigned int i = 0; i < ITERATIONS; ++i) {
    std::unordered_map<coord, bool, boost::hash<coord>> point_map;

    for (unsigned int j = 0; j < ITERATIONS; ++j) {
      for (const auto &point : points) {
        point_map[point] = true;
      }

      for (const auto &point : point_map) {
        if (point.second) {
          p_result += 1;
        }
      }

      // p_result += static_cast<unsigned long>(std::count_if(point_map.begin(), point_map.end(), [](const auto &point) { return point.second; }));

      for (const auto &point : point_map) {
        const auto inverse = std::make_pair(point.first.second, point.first.first);
        if (((point_map[inverse] ? 2 : 3) % (i + 1)) != 0) {
          point_map[std::make_pair(point.first.first, point.first.second)] = false;
        }
      }


      for (const auto &point : point_map) {
        if (point.second) {
          p_reflect_result += 1;
        }
      }

      // p_reflect_result += static_cast<unsigned long>(std::count_if(point_map.begin(), point_map.end(), [](const auto &point) { return point.second; }));
    }
  }

  auto p_end = std::chrono::high_resolution_clock::now();


  // Double map version
  auto d_start = std::chrono::high_resolution_clock::now();

  unsigned long d_result = 0;
  unsigned long d_reflect_result = 0;

  for (unsigned int i = 0; i < ITERATIONS; ++i) {
    std::unordered_map<unsigned int, std::unordered_map<unsigned int, bool>> point_map;

    for (unsigned int j = 0; j < ITERATIONS; ++j) {
      for (const auto &point : points) {
        point_map[point.first][point.second] = true;
      }

      for (const auto &x_row : point_map) {
        for (const auto &y : x_row.second) {
          if (y.second) {
            d_result += 1;
          }
        }
      }

      for (const auto &x_row : point_map) {
        for (const auto &y : x_row.second) {
          if (((point_map[y.first][x_row.first] ? 2 : 3) % (i + 1)) != 0) {
            point_map[x_row.first][y.first] = false;
          }
        }
      }


      for (const auto &x_row : point_map) {
        for (const auto &y : x_row.second) {
          if (y.second) {
            d_reflect_result += 1;
          }
        }
      }
    }
  }

  auto d_end = std::chrono::high_resolution_clock::now();

  std::cout << "[Pair] Result: " << p_result << "\nReflected result: " << p_reflect_result << std::endl;

  std::cout << "[Double map] Result: " << d_result << "\nReflected result: " << d_reflect_result << std::endl;

  std::cout
    << "\nPair version took "
    << std::chrono::duration_cast<std::chrono::duration<double>>(p_end - p_start).count() << " seconds\n"
    << "\nDouble map version took "
    << std::chrono::duration_cast<std::chrono::duration<double>>(d_end - d_start).count() << " seconds\n"
    << std::endl;

  return p_reflect_result + d_reflect_result;
}

TEST_CASE("testing solver for day 13 part 1 - dotted paper folding")
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

  CHECK(solver.solve(is) == 17);
}
