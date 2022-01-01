#include <fstream>
#include <functional>
#include <iostream>
#include <map>

#include <docopt/docopt.h>
#include <spdlog/spdlog.h>

#include "03/part1/solver_03_part1.h"
#include "03/part2/solver_03_part2.h"

static constexpr auto USAGE =
  R"(Advent of Code.

    Usage:
          advent_of_code <day> <part> <file>
          advent_of_code (-h | --help)
          advent_of_code --version
 Options:
          -h --help     Show this screen.
          --version     Show version.
)";


int main(int argc, const char **argv)
{
  std::map<std::string, docopt::value> args = docopt::docopt(USAGE,
    { std::next(argv), std::next(argv, argc) },
    true,// show help if requested
    "Advent of Code 1.0");// version string

  auto day = args["<day>"].asString();
  auto part = args["<part>"].asString();
  auto inputFilePath = args["<file>"].asString();

  std::ifstream inputFile(inputFilePath);

  if (!inputFile.is_open()) {
    std::cerr << "Could not open input file: " << inputFilePath << std::endl;
    return 1;
  }

  if (inputFile.peek() == EOF) {
    std::cerr << "Input fild invalid or empty: " << inputFilePath << std::endl;
    return 1;
  }

  Solver *solver = nullptr;

  if (day == "three") {
    if (part == "one") {
      solver = new Solver_03_part1;
    } else if (part == "two") {
      solver = new Solver_03_part2;
    }
  }

  if (solver == nullptr) {
    spdlog::error("No solver for '{}'", day);
    return 1;
  }

  unsigned long result = solver->solve(inputFile);

  std::cout << "Result: " << result << std::endl;

  return 0;
}
