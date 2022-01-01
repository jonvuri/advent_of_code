#include <fstream>
#include <functional>
#include <iostream>
#include <map>

#include <docopt/docopt.h>
#include <spdlog/spdlog.h>

#include "01/part1/solver_01_part1.h"
#include "03/part1/solver_03_part1.h"
#include "03/part2/solver_03_part2.h"

static constexpr auto USAGE =
  R"(Advent of Code.

    Usage:
          advent_of_code <day> <file>
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

  docopt::value dayArg = args["<day>"],
                partArg = args["<part>"],
                inputFilePathArg = args["<file>"];

  long day, part;
  std::string inputFilePath;

  try {
    day = dayArg ? dayArg.asLong() : 0;
    part = partArg ? partArg.asLong() : 0;
    inputFilePath = inputFilePathArg ? inputFilePathArg.asString() : "";
  } catch (...) {
    spdlog::error("Invalid arguments - ensure <day> and <part> are numbers and <file> is a valid path to a text file.");
    return 1;
  }


  if (inputFilePath.empty()) {
    spdlog::error("<file> cannot be empty - enter a path to a valid input file.");
    return 1;
  }

  std::ifstream inputFile(inputFilePath);

  if (!inputFile.is_open()) {
    spdlog::error("Could not open input file: {}", inputFilePath);
    return 1;
  }

  if (inputFile.peek() == EOF) {
    spdlog::error("Input file invalid or empty: {}", inputFilePath);
    return 1;
  }

  Solver *solver = nullptr;

  if (day == 1) {
    solver = new Solver_01_part1;
  } else if (day == 3) {
    if (part == 1) {
      solver = new Solver_03_part1;
    } else if (part == 2) {
      solver = new Solver_03_part2;
    }
  }

  if (solver == nullptr) {
    if (part != 0) {
      spdlog::error("No solver for day {} part {}", day, part);
    } else {
      spdlog::error("No solver for day {}", day);
    }
    return 1;
  }

  unsigned long result = solver->solve(inputFile);

  std::cout << "Result: " << result << std::endl;

  return 0;
}
