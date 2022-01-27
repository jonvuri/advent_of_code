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

#include "solver_15_part1.h"


namespace {

using Coordinate = std::pair<unsigned int, unsigned int>;

class Vertex
{
private:
  Coordinate position_;
  Coordinate parent_position_;// Next vertex back that continues current lowest cost path that was found
  unsigned int g_ = std::numeric_limits<unsigned int>::max();// Current lowest total cost to travel from start to this vertex (including this vertex's cost)
  unsigned int h_;// Pre-computed heuristic - Manhattan distance from vertex to end (equal to shortest path from this vertex to end if all costs are 1)
  unsigned int cost_;// Cost of entering this vertex

  // Lowest g + h = f is the next vertex we consider in the search
  [[nodiscard]] unsigned int f() const
  {
    return g_ + h_;
  };

public:
  explicit Vertex(const Coordinate position, const unsigned int cost, const unsigned int h)
    : position_(position), h_(h), cost_(cost) {}

  // Used for priority queue minimum
  friend bool operator>(const Vertex &lhs, const Vertex &rhs) { return lhs.f() > rhs.f(); }

  [[nodiscard]] Coordinate position() const
  {
    return position_;
  }

  Coordinate &parent_position() { return parent_position_; }

  unsigned int &g() { return g_; }
  [[nodiscard]] unsigned int g() const { return g_; }

  unsigned int &cost() { return cost_; }
};

std::vector<Coordinate> getNeighborCoords(Coordinate center, unsigned int width, unsigned int height)
{
  std::vector<Coordinate> neighbors;

  const auto &x = center.first;
  const auto &y = center.second;

  if (x > 0) {
    neighbors.emplace_back(std::make_pair(x - 1, y));
  }

  if (y > 0) {
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


std::string Solver_15_part1::solve(std::istream &is)
{
  std::vector<std::tuple<unsigned int, unsigned int, unsigned int>> inputs;

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

        inputs.emplace_back(std::make_tuple(x, y, cost));

        x += 1;
      }
    }
    height = x == 0 ? y : y + 1;
  }

  unsigned int goal_x = width - 1;
  unsigned int goal_y = height - 1;

  Coordinate start = std::make_pair(0, 0);
  Coordinate goal = std::make_pair(goal_x, goal_y);

  // Now that we know the width / height we can precompute h (distance) and populate the full map
  std::unordered_map<Coordinate, Vertex, boost::hash<Coordinate>> cave_map;

  for (const auto &[x, y, cost] : inputs) {
    const Coordinate position = std::make_pair(x, y);

    // Manhattan distance heuristic for 4-movement grid: |goal_x - vertex_x| + |goal_y - vertex_y|
    // We don't need to do abs (||) op, because we assume the goal is always at the bottom right corner
    const auto h = (goal_x - x) + (goal_y - y);

    cave_map.emplace(position, Vertex(position, cost, h));
  }

  std::priority_queue<Vertex, std::vector<Vertex>, std::greater<>> open_list;
  std::unordered_set<Coordinate, boost::hash<Coordinate>> open_set;
  std::unordered_set<Coordinate, boost::hash<Coordinate>> closed_set;

  cave_map.at(start).g() = 0;
  open_list.push(cave_map.at(start));// No need to place it on set too, it will just be immediately erased

  // Use A* search to find guaranteed shortest path
  // Assuming A* w/ Manhattan distance will work here might be incorrect? It solved part 1
  // but brought unexpected problems in part 2.
  while (!open_list.empty()) {
    const Vertex vertex = open_list.top();
    open_list.pop();
    open_set.erase(vertex.position());

    if (vertex.position() == goal) {
      // Goal found! Trace path and end
      unsigned int cost = 0;

      // Don't count start cost
      for (Coordinate c = vertex.position(); c != start; c = cave_map.at(c).parent_position()) {
        cost += cave_map.at(c).cost();
      }

      return std::to_string(cost);
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
        neighbor_vertex.parent_position() = vertex.position();
      }

      if (closed_set.find(neighbor_coord) == closed_set.end() && open_set.find(neighbor_coord) == open_set.end()) {
        open_list.push(neighbor_vertex);
        open_set.insert(neighbor_coord);
      }
    }
  }

  throw solver_runtime_error("Could not find a path to the goal");
}

TEST_CASE("testing solver for day 15 part 1 - path through chiton cave")
{
  Solver_15_part1 solver;

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

    CHECK(solver.solve(is) == "40");
  }


  {
    std::istringstream is(std::string{ R"(
      19999
      19111
      11191
    )" });

    CHECK(solver.solve(is) == "8");
  }

  {
    std::istringstream is(std::string{ R"(
      59999
      19111
      11195
    )" });

    CHECK(solver.solve(is) == "12");
  }
}
