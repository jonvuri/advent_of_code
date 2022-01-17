#include <algorithm>
#include <limits>
#include <list>
#include <string>
#include <sstream>
#include <unordered_map>
#include <vector>

#include <doctest/doctest.h>

#include "solver_14_part1.h"


namespace {

struct PairInsertionRule
{
  const char left;
  const char right;
  const char insert;
};

const unsigned int ITERATIONS = 10;

}// namespace


// Assumptions:
// - There won't be more than one rule for a given left/right pair of characters
// - Thus, there will be exactly zero or one rules matching a given pair
std::string Solver_14_part1::solve(std::istream &is)
{
  std::list<char> polymer;
  std::vector<PairInsertionRule> rules;

  is >> std::noskipws >> std::ws;

  // Ingest initial polymer
  {
    char input = '\0';
    while (is >> input && isspace(input) == 0) {
      polymer.push_back(input);
    }
  }

  // Ingest pair insertion rules
  {
    char left = '\0';
    char right = '\0';
    char insert = '\0';

    while (is >> std::ws && is >> left && is >> right && is.ignore(4) && is >> insert) {
      rules.push_back({ left, right, insert });
    }
  }

  // Perform ITERATIONS number of insertions on the whole list at once (that is, skipping characters after inserting them in one iteration)
  for (unsigned int i = 0; i < ITERATIONS; ++i) {
    for (auto iter = polymer.begin(), next = std::next(iter); iter != polymer.end() && next != polymer.end(); next = std::next(++iter)) {
      const auto left = *iter;
      const auto right = *next;

      const auto match = std::find_if(rules.begin(), rules.end(), [&](const auto &rule) { return left == rule.left && right == rule.right; });

      if (match != rules.end()) {
        iter = polymer.insert(next, match->insert);
      }
    }
  }

  // Count results from final polymer
  unsigned int min = std::numeric_limits<unsigned int>::max();
  unsigned int max = 0;

  {
    std::unordered_map<char, unsigned int> char_counts;

    for (const auto &ch : polymer) {
      char_counts[ch] += 1;
    }
    for (const auto &count : char_counts) {
      if (count.second < min) {
        min = count.second;
      }
      if (count.second > max) {
        max = count.second;
      }
    }
  }

  return std::to_string(max - min);
}

TEST_CASE("testing solver for day 14 part 1 - polymerization")
{
  Solver_14_part1 solver;

  std::istringstream is(std::string{ R"(
    NNCB

    CH -> B
    HH -> N
    CB -> H
    NH -> C
    HB -> C
    HC -> B
    HN -> C
    NN -> C
    BH -> H
    NC -> B
    NB -> B
    BN -> B
    BB -> N
    BC -> B
    CC -> N
    CN -> C
  )" });

  CHECK(solver.solve(is) == "1588");
}
