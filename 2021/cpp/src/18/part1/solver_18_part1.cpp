#include <algorithm>
#include <array>
#include <cctype>
#include <iterator>
#include <list>
#include <numeric>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include <doctest/doctest.h>

#include "solver_18_part1.h"


namespace {

const int MAX_DEPTH = 4;

struct Element
{
  int depth;
  long value;
};


bool maybe_explode(std::list<Element> &elements)
{
  for (auto it = elements.begin(); it != elements.end(); ++it) {
    if (it->depth > MAX_DEPTH) {
      // Add left element to next leftmost element, if applicable
      if (it != elements.begin()) {
        std::prev(it)->value += it->value;
      }

      // Reset left element to stay as the 0
      it->value = 0;
      it->depth -= 1;

      // Shift to right and add to next rightmost if applicable
      const auto right = ++it;
      if (right != elements.end()) {
        std::next(right)->value += right->value;
      }

      elements.erase(right);

      return true;
    }
  }

  return false;
}

bool maybe_split(std::list<Element> &elements)
{
  for (auto it = elements.begin(); it != elements.end(); ++it) {
    if (it->value >= 10) {
      // Create new left element
      Element left{ it->depth + 1, it->value / 2 };

      // Reuse existing right element
      it->depth += 1;
      it->value = (it->value / 2) + (it->value % 2);

      elements.insert(it, left);

      return true;
    }
  }

  return false;
}


std::list<Element> &add_elements(std::list<Element> &lhs, std::list<Element> &rhs)
{
  // No need to reduce
  if (lhs.empty()) {
    return rhs;
  } else if (rhs.empty()) {
    return lhs;
  }

  lhs.splice(lhs.end(), rhs);

  for (auto &element : lhs) {
    element.depth += 1;
  }

  // Reduce number
  while (maybe_explode(lhs) || maybe_split(lhs)) {}

  return lhs;
}

long magnitude(const std::list<Element> &number)
{
  static const long EMPTY = -1;

  // Stack of dangling left pair elements that will fully cascade down to 0 as number is fully iterated
  std::array<long, 5> stack = {};
  stack.fill(EMPTY);

  for (const auto &element : number) {
    auto depth = static_cast<unsigned long>(element.depth);

    if (stack.at(depth) == EMPTY) {
      // Store dangling left element
      stack.at(depth) = element.value;
    } else {
      // Combine left + right element, then store it in next depth downwards - if there is a left element waiting there, repeat etc.
      auto i = depth;
      auto pair_magnitude = element.value;
      for (; i > 0 && stack.at(i) != EMPTY; --i) {
        pair_magnitude = stack.at(i) * 3 + pair_magnitude * 2;
        stack.at(i) = EMPTY;
      }
      stack.at(i) = pair_magnitude;
    }
  }

  // Empty case not defined
  return stack[0];
}

}// namespace


std::string Solver_18_part1::solve(std::istream &is)
{
  std::vector<std::list<Element>> numbers;

  std::string line;
  while (std::getline(is >> std::ws, line)) {
    std::list<Element> number;
    int depth = 0;

    for (const auto ch : line) {
      if (ch == '[') {
        depth += 1;
      } else if (ch == ']') {
        if (depth <= 0) {
          throw solver_runtime_error("Invalid number of brackets in input");
        }
        depth -= 1;
      } else if (std::isdigit(ch) != 0) {
        number.emplace_back(Element{ depth, static_cast<long>(ch - '0') });
      }
    }

    numbers.push_back(std::move(number));
  }

  return std::to_string(
    magnitude(
      std::accumulate(numbers.begin(), numbers.end(), std::list<Element>(), add_elements)));
}

TEST_CASE("testing solver for day 18 part 1 - snailfish numbers")
{
  Solver_18_part1 solver;

  std::istringstream is(std::string{ R"(
      [[[0,[5,8]],[[1,7],[9,6]]],[[4,[1,2]],[[1,4],2]]]
      [[[5,[2,8]],4],[5,[[9,9],0]]]
      [6,[[[6,2],[5,6]],[[7,6],[4,7]]]]
      [[[6,[0,7]],[0,9]],[4,[9,[9,0]]]]
      [[[7,[6,4]],[3,[1,3]]],[[[5,5],1],9]]
      [[6,[[7,3],[3,2]]],[[[3,8],[5,7]],4]]
      [[[[5,4],[7,7]],8],[[8,3],8]]
      [[9,3],[[9,9],[6,[4,9]]]]
      [[2,[[7,7],7]],[[5,8],[[9,3],[0,2]]]]
      [[[[5,2],5],[8,[3,7]]],[[5,[7,5]],[4,4]]]
    )" });

  CHECK(solver.solve(is) == "4140");
}
