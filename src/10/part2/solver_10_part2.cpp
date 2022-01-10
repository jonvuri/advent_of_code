#include <algorithm>
#include <array>
#include <cctype>
#include <limits>
#include <string>
#include <sstream>
#include <stack>
#include <vector>

#include <doctest/doctest.h>

#include "solver_10_part2.h"


unsigned long
  Solver_10_part2::solve(std::istream &is)
{
  std::vector<unsigned long> scores;

  std::string line;
  while (std::getline(is, line)) {
    std::stack<char> chunk_chars;
    bool error = false;

    for (const auto &ch : line) {
      if (std::isspace(ch) != 0) {
        continue;
      }

      switch (ch) {
      case '(':
      case '[':
      case '{':
      case '<':
        chunk_chars.push(ch);
        continue;
      default:
        break;
      }

      const auto &top_char = chunk_chars.top();
      chunk_chars.pop();

      switch (ch) {
      case ')':
        if (top_char == '(') {
          continue;
        } else {
          break;
        }
      case ']':
        if (top_char == '[') {
          continue;
        } else {
          break;
        }
      case '}':
        if (top_char == '{') {
          continue;
        } else {
          break;
        }
      case '>':
        if (top_char == '<') {
          continue;
        } else {
          break;
        }
      default:
        break;
      }

      error = true;
      break;
    }

    if (!error) {
      unsigned long line_score = 0;
      while (!chunk_chars.empty()) {
        const auto &top_char = chunk_chars.top();
        chunk_chars.pop();

        line_score *= 5;
        switch (top_char) {
        case '(':
          line_score += 1;
          break;
        case '[':
          line_score += 2;
          break;
        case '{':
          line_score += 3;
          break;
        case '<':
          line_score += 4;
          break;
        default:
          break;
        }
      }

      if (line_score > 0) {
        scores.push_back(line_score);
      }
    }
  }

  unsigned long n = scores.size() / 2;
  std::nth_element(scores.begin(), scores.begin() + static_cast<long>(n), scores.end());
  return scores[n];
}

TEST_CASE("testing solver for day 10 part 2 - syntax parser autocomplete")
{
  Solver_10_part2 solver;
  std::istringstream is(std::string{ R"(
    [({(<(())[]>[[{[]{<()<>>
    [(()[<>])]({[<{<<[]>>(
    {([(<{}[<>[]}>{[]{[(<()>
    (((({<>}<{<{<>}{[]{[]{}
    [[<[([]))<([[{}[[()]]]
    [{[{({}]{}}([{[{{{}}([]
    {<[[]]>}<{[{[{[]{()[[[]
    [<(<(<(<{}))><([]([]()
    <{([([[(<>()){}]>(<<{{
    <{([{{}}[<[[[<>{}]]]>[]]
  )" });

  CHECK(solver.solve(is) == 288957);
}
