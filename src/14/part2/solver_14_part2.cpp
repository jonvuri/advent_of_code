#include <algorithm>
#include <boost/functional/hash.hpp>
#include <limits>
#include <string>
#include <sstream>
#include <unordered_map>
#include <vector>

#include <doctest/doctest.h>

#include "solver_14_part2.h"


namespace {

using CharPair = std::pair<char, char>;

class PairExpansion
{
private:
  // Construct pairs ahead of time since they're the map keys
  CharPair left_pair_;
  CharPair right_pair_;
  char insert_;// Center character, the only "new" one added to the polymer from the two overlapping pairs

public:
  PairExpansion(char l, char i, char r) : left_pair_(std::make_pair(l, i)), right_pair_(std::make_pair(i, r)), insert_(i)
  {}

  [[nodiscard]] const CharPair &left_pair() const { return left_pair_; }
  [[nodiscard]] const CharPair &right_pair() const { return right_pair_; }
  [[nodiscard]] const char &insert() const { return insert_; }
};

const unsigned int ITERATIONS = 40;

}// namespace


// Assumptions:
// - There won't be more than one rule for a given left/right pair of characters
// - Thus, there will be exactly zero or one rules matching a given pair
std::string Solver_14_part2::solve(std::istream &is)
{
  std::vector<char> polymer;
  std::unordered_map<CharPair, PairExpansion, boost::hash<CharPair>> pair_expansions;
  std::unordered_map<CharPair, unsigned long long, boost::hash<CharPair>> source_pair_counts;
  std::unordered_map<char, unsigned long long> char_counts;

  is >> std::noskipws >> std::ws;

  // Ingest initial polymer
  {
    char input = '\0';
    while (is >> input && isspace(input) == 0) {
      polymer.push_back(input);
      char_counts[input] += 1;
    }
  }

  // Ingest pair insertion rules
  {
    char left = '\0';
    char right = '\0';
    char insert = '\0';

    while (is >> std::ws && is >> left && is >> right && is.ignore(4) && is >> insert) {
      PairExpansion expansion(left, insert, right);
      pair_expansions.emplace(std::make_pair(left, right), expansion);
    }
  }


  for (auto iter = polymer.begin(), next = std::next(iter); iter != polymer.end() && next != polymer.end(); next = std::next(++iter)) {
    source_pair_counts[std::make_pair(*iter, *next)] += 1;
  }

  for (unsigned int i = 0; i < ITERATIONS; ++i) {
    std::unordered_map<CharPair, unsigned long long, boost::hash<CharPair>> dest_pair_counts;

    for (const auto &pair_count : source_pair_counts) {
      const auto &pair = pair_count.first;
      const auto &count = pair_count.second;
      if (count > 0) {
        const auto &expansion_iter = pair_expansions.find(pair);
        if (expansion_iter != pair_expansions.end()) {
          const auto &expansion = expansion_iter->second;

          dest_pair_counts[expansion.left_pair()] += count;
          dest_pair_counts[expansion.right_pair()] += count;
          char_counts[expansion.insert()] += count;
        } else {
          dest_pair_counts[pair] += count;
        }
      }
    }

    source_pair_counts = dest_pair_counts;
  }

  unsigned long long min = std::numeric_limits<unsigned long long>::max();
  unsigned long long max = 0;
  {
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

TEST_CASE("testing solver for day 14 part 22- polymerization")
{
  Solver_14_part2 solver;

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

  CHECK(solver.solve(is) == "2188189693529");
}
