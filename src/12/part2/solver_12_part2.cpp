#include <algorithm>
#include <array>
#include <cctype>
#include <queue>
#include <string>
#include <sstream>
#include <unordered_map>
#include <vector>

#include <doctest/doctest.h>

#include "solver_12_part2.h"


// Assumptions:
// - Input list will never contain duplicate or mirrored duplicate edges (e.g. "A-b" and "b-A")
// - Input list will always contain "start" and "end" nodes
// - "Big" caves can be determined by just the first letter being uppercase
unsigned long Solver_12_part2::solve(std::istream &is)
{
  static const std::string START = "start";
  static const std::string END = "end";

  std::vector<std::pair<std::string, std::string>> edges;
  std::unordered_map<std::string, std::vector<std::string>> neighbor_map;

  std::string node1;
  std::string node2;
  while (is >> std::ws && std::getline(is, node1, '-') && std::getline(is, node2)) {
    neighbor_map[node1].push_back(node2);
    neighbor_map[node2].push_back(node1);

    edges.emplace_back(std::make_pair(node1, node2));
  }

  std::vector<std::vector<std::string>> paths;
  std::queue<std::vector<std::string>> incomplete_paths;

  for (const auto &neighbor : neighbor_map[START]) {
    incomplete_paths.push({ START, neighbor });
  }

  while (!incomplete_paths.empty()) {
    const auto &path = incomplete_paths.front();

    const auto &path_tail = path.back();
    const auto &tail_neighbors = neighbor_map[path_tail];

    for (const auto &tail_neighbor : tail_neighbors) {
      if (tail_neighbor == START) {
        continue;
      }

      if (tail_neighbor == END) {
        std::vector<std::string> complete_path(path);
        complete_path.emplace_back(END);

        paths.push_back(std::move(complete_path));
      } else if (std::isupper(tail_neighbor[0]) != 0) {
        // Big cave, append it unconditionally - doesn't matter if we visited before
        std::vector<std::string> incomplete_path(path);
        incomplete_path.emplace_back(tail_neighbor);

        incomplete_paths.push(std::move(incomplete_path));
      } else {
        // Small cave, must check that no other small cave has already been visited twice and this one visited once
        std::unordered_map<std::string, unsigned int> smalls_visited;

        for (const auto &node : path) {
          if (node == START || std::isupper(node[0]) != 0) {
            continue;
          }

          smalls_visited[node] = smalls_visited[node] + 1;
        }

        bool other_small_visited_twice =
          std::find_if(smalls_visited.begin(), smalls_visited.end(), [&](const std::unordered_map<std::string, unsigned int>::value_type &kv) { return kv.first != tail_neighbor && kv.second > 1; })
          != smalls_visited.end();

        if (!(other_small_visited_twice && smalls_visited[tail_neighbor] > 0) && !(smalls_visited[tail_neighbor] > 1)) {
          std::vector<std::string> incomplete_path(path);
          incomplete_path.emplace_back(tail_neighbor);

          incomplete_paths.push(std::move(incomplete_path));
        }
      }
    }

    incomplete_paths.pop();
  }

  return paths.size();
}

TEST_CASE("testing solver for day 12 part 2 - all paths through caves, one small twice")
{
  Solver_12_part2 solver;

  {
    std::istringstream is(std::string{ R"(
      start-A
      start-b
      A-c
      A-b
      b-d
      A-end
      b-end
    )" });

    CHECK(solver.solve(is) == 36);
  }

  {
    std::istringstream is(std::string{ R"(
      dc-end
      HN-start
      start-kj
      dc-start
      dc-HN
      LN-dc
      HN-end
      kj-sa
      kj-HN
      kj-dc
    )" });

    CHECK(solver.solve(is) == 103);
  }
}
