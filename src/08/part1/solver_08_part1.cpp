#include <limits>
#include <string>
#include <sstream>
#include <vector>

#include <doctest/doctest.h>

#include "solver_08_part1.h"


unsigned long
  Solver_08_part1::solve(std::istream &is)
{
  unsigned long unique_digits = 0;

  std::string line;
  while (std::getline(is, line)) {
    std::istringstream iss(line);

    std::vector<std::string> output_digits;

    iss.ignore(std::numeric_limits<std::streamsize>::max(), '|');
    std::string input;
    while (iss >> input) {
      output_digits.push_back(input);
    }

    for (const auto &digit : output_digits) {
      switch (digit.length()) {
      case 2:// Digit '1'
      case 3:// Digit '7'
      case 4:// Digit '4'
      case 7:// Digit '8'
        unique_digits++;
      }
    }
  }

  return unique_digits;
}

TEST_CASE("testing solver for day 8 part 1 - seven segment display decoding and counting unique-length digits only")
{
  Solver_08_part1 solver;
  std::istringstream is(std::string{ R"(
    be cfbegad cbdgef fgaecd cgeb fdcge agebfd fecdb fabcd edb | fdgacbe cefdb cefbgd gcbe
    edbfga begcd cbg gc gcadebf fbgde acbgfd abcde gfcbed gfec | fcgedb cgb dgebacf gc
    fgaebd cg bdaec gdafb agbcfd gdcbef bgcad gfac gcb cdgabef | cg cg fdcagb cbg
    fbegcd cbd adcefb dageb afcb bc aefdc ecdab fgdeca fcdbega | efabcd cedba gadfec cb
    aecbfdg fbg gf bafeg dbefa fcge gcbea fcaegb dgceab fcbdga | gecf egdcabf bgf bfgea
    fgeab ca afcebg bdacfeg cfaedg gcfdb baec bfadeg bafgc acf | gebdcfa ecba ca fadegcb
    dbcfg fgd bdegcaf fgec aegbdf ecdfab fbedc dacgb gdcebf gf | cefg dcbef fcge gbcadfe
    bdfegc cbegaf gecbf dfcage bdacg ed bedf ced adcbefg gebcd | ed bcgafe cdgba cbgef
    egadfb cdbfeg cegd fecab cgb gbdefca cg fgcdab egfdb bfceg | gbdfcae bgc cg cgb
    gcafb gcf dcaebfg ecagb gf abcdeg gaef cafbge fdbac fegbdc | fgae cfgab fg bagce
  )" });

  CHECK(solver.solve(is) == 26);
}
