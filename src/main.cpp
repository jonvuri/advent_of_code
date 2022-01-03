#include <fstream>
#include <functional>
#include <iostream>
#include <map>

#include <docopt/docopt.h>
#include <spdlog/spdlog.h>

#include "01/part1/solver_01_part1.h"
#include "01/part2/solver_01_part2.h"
#include "02/part1/solver_02_part1.h"
#include "02/part2/solver_02_part2.h"
#include "03/part1/solver_03_part1.h"
#include "03/part2/solver_03_part2.h"
#include "04/part1/solver_04_part1.h"
#include "04/part2/solver_04_part2.h"
#include "05/part1/solver_05_part1.h"
#include "05/part2/solver_05_part2.h"
#include "06/part1/solver_06_part1.h"
#include "06/part2/solver_06_part2.h"
#include "07/part1/solver_07_part1.h"
#include "07/part2/solver_07_part2.h"

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
    part = partArg ? partArg.asLong() : 1;
    inputFilePath = inputFilePathArg ? inputFilePathArg.asString() : "";
  } catch (...) {
    spdlog::error("Invalid arguments - ensure <day> and <part> are numbers and <file> is a valid path to a text file.");
    return 1;
  }

  if (day <= 0) {
    spdlog::error("<day> must be a positive number.");
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
    if (part == 1) {
      solver = new Solver_01_part1;
    } else if (part == 2) {
      solver = new Solver_01_part2;
    }
  } else if (day == 2) {
    if (part == 1) {
      solver = new Solver_02_part1;
    } else if (part == 2) {
      solver = new Solver_02_part2;
    }
  } else if (day == 3) {
    if (part == 1) {
      solver = new Solver_03_part1;
    } else if (part == 2) {
      solver = new Solver_03_part2;
    }
  } else if (day == 4) {
    if (part == 1) {
      solver = new Solver_04_part1;
    } else if (part == 2) {
      solver = new Solver_04_part2;
    }
  } else if (day == 5) {
    if (part == 1) {
      solver = new Solver_05_part1;
    } else if (part == 2) {
      solver = new Solver_05_part2;
    }
  } else if (day == 6) {
    if (part == 1) {
      solver = new Solver_06_part1;
    } else if (part == 2) {
      solver = new Solver_06_part2;
    }
  } else if (day == 7) {
    if (part == 1) {
      solver = new Solver_07_part1;
    } else if (part == 2) {
      solver = new Solver_07_part2;
    }
  }

  if (solver == nullptr) {
    spdlog::error("No solver for day {} part {}", day, part);
    return 1;
  }

  unsigned long result = solver->solve(inputFile);

  std::cout << "Result: " << result << std::endl;

  return 0;
}
