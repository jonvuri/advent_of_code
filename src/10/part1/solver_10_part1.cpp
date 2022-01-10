#include <array>
#include <cctype>
#include <limits>
#include <string>
#include <sstream>
#include <stack>
#include <vector>

#include <doctest/doctest.h>

#include "solver_10_part1.h"


unsigned long
  Solver_10_part1::solve(std::istream &is)
{
  unsigned long error_score = 0;

  std::string line;
  while (std::getline(is, line)) {
    std::stack<char> chunk_chars;

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
          error_score += 3;
          break;
        }
      case ']':
        if (top_char == '[') {
          continue;
        } else {
          error_score += 57;
          break;
        }
      case '}':
        if (top_char == '{') {
          continue;
        } else {
          error_score += 1197;
          break;
        }
      case '>':
        if (top_char == '<') {
          continue;
        } else {
          error_score += 25137;
          break;
        }
      default:
        break;
      }

      // We did not continue - error or invalid character found, so stop processing this line
      break;
    }
  }

  return error_score;
}

TEST_CASE("testing solver for day 10 part 1 - syntax checker")
{
  Solver_10_part1 solver;
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

  CHECK(solver.solve(is) == 26397);
}
