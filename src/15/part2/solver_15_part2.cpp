#include <algorithm>
#include <boost/functional/hash.hpp>
#include <functional>
#include <limits>
#include <queue>
#include <sstream>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include <doctest/doctest.h>

#include "solver_15_part2.h"


namespace {

using Coordinate = std::pair<unsigned int, unsigned int>;

class Vertex
{
private:
  Coordinate position_;
  unsigned int g_ = std::numeric_limits<unsigned int>::max();// Current lowest total cost to travel from start to this vertex (including this vertex's cost)
  unsigned int cost_;// Cost of entering this vertex

public:
  explicit Vertex(const Coordinate position, const unsigned int cost)
    : position_(position), cost_(cost) {}

  // Used for priority queue minimum
  friend bool operator>(const Vertex &lhs, const Vertex &rhs) { return lhs.g() > rhs.g(); }

  [[nodiscard]] Coordinate position() const
  {
    return position_;
  }

  unsigned int &g() { return g_; }
  [[nodiscard]] unsigned int g() const { return g_; }

  unsigned int &cost() { return cost_; }
};

std::vector<Coordinate> getNeighborCoords(Coordinate center, unsigned int width, unsigned int height)
{
  std::vector<Coordinate> neighbors;

  const auto &x = center.first;
  const auto &y = center.second;

  // Don't bother returning the start vertex at 0,0
  if (y > 0 ? x > 0 : x > 1) {
    neighbors.emplace_back(std::make_pair(x - 1, y));
  }

  if (x > 0 ? y > 0 : y > 1) {
    neighbors.emplace_back(std::make_pair(x, y - 1));
  }

  if (x < width - 1) {
    neighbors.emplace_back(std::make_pair(x + 1, y));
  }

  if (y < height - 1) {
    neighbors.emplace_back(std::make_pair(x, y + 1));
  }

  return neighbors;
}

}// namespace


std::string Solver_15_part2::solve(std::istream &is)
{
  std::vector<std::tuple<unsigned int, unsigned int, unsigned int>> base_inputs;

  unsigned int width = 0;
  unsigned int height = 0;

  // Ingest the map
  {
    unsigned int x = 0;
    unsigned int y = 0;

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
        int cost = input - '0';
        if (cost < 0 || cost > 9) {
          throw solver_runtime_error("Input out of range");
        }

        base_inputs.emplace_back(std::make_tuple(x, y, cost));

        x += 1;
      }
    }
    height = x == 0 ? y : y + 1;
  }

  std::vector<std::tuple<unsigned int, unsigned int, unsigned int>> inputs;

  // Duplicate the map 4 times to the right and downward
  for (const auto &[x, y, cost] : base_inputs) {
    inputs.emplace_back(x, y, cost);
    for (unsigned int cell_y = 0; cell_y < 5; ++cell_y) {
      for (unsigned int cell_x = 0; cell_x < 5; ++cell_x) {
        if (cell_x == 0 and cell_y == 0) {
          continue;
        }

        inputs.emplace_back(std::make_tuple(
          x + width * cell_x,
          y + height * cell_y,
          ((cost + cell_x + cell_y - 1) % 9) + 1));
      }
    }
  }

  width *= 5;
  height *= 5;

  unsigned int goal_x = width - 1;
  unsigned int goal_y = height - 1;

  Coordinate start = std::make_pair(0, 0);
  Coordinate goal = std::make_pair(goal_x, goal_y);

  std::unordered_map<Coordinate, Vertex, boost::hash<Coordinate>> cave_map;

  for (const auto &[x, y, cost] : inputs) {
    const Coordinate position = std::make_pair(x, y);
    cave_map.emplace(position, Vertex(position, cost));
  }

  std::priority_queue<Vertex, std::vector<Vertex>, std::greater<>> open_list;
  std::unordered_set<Coordinate, boost::hash<Coordinate>> open_set;
  std::unordered_set<Coordinate, boost::hash<Coordinate>> closed_set;

  cave_map.at(start).g() = 0;
  open_list.push(cave_map.at(start));// No need to place it on set too, it will just be immediately erased

  // Use BFS algorithm to find the shortest path
  while (!open_list.empty()) {
    const Vertex vertex = open_list.top();
    open_list.pop();
    open_set.erase(vertex.position());

    if (vertex.position() == goal) {
      // Goal found! Trace path and end
      return std::to_string(vertex.g());
    }

    // Only visit every vertex once
    if (closed_set.find(vertex.position()) != closed_set.end()) {
      continue;
    }
    closed_set.insert(vertex.position());

    // Now look at all vertex's neighbors and update their g if new g through this vertex is lower (even if neighbor's been visited)
    for (const auto &neighbor_coord : getNeighborCoords(vertex.position(), width, height)) {
      auto &neighbor_vertex = cave_map.at(std::make_pair(neighbor_coord.first, neighbor_coord.second));

      const auto new_g = neighbor_vertex.cost() + vertex.g();

      if (new_g < neighbor_vertex.g()) {
        neighbor_vertex.g() = new_g;

        if (open_set.find(neighbor_coord) == open_set.end()) {
          open_list.push(neighbor_vertex);
          open_set.insert(neighbor_coord);
        }
      }
    }
  }

  throw solver_runtime_error("Could not find a path to the goal");
}

TEST_CASE("testing solver for day 15 part 2 - path through chiton cave expanded 5 times right/downward")
{
  Solver_15_part2 solver;

  {
    std::istringstream is(std::string{ R"(
      1163751742
      1381373672
      2136511328
      3694931569
      7463417111
      1319128137
      1359912421
      3125421639
      1293138521
      2311944581
    )" });

    CHECK(solver.solve(is) == "315");
  }

  {
    std::istringstream is(std::string{ R"(
      19999
      19111
      11191
    )" });

    CHECK(solver.solve(is) == "158");
  }

  {
    std::istringstream is(std::string{ R"(
      59999
      19111
      11195
    )" });

    CHECK(solver.solve(is) == "138");
  }
}
