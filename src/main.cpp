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
#include "08/part1/solver_08_part1.h"
#include "08/part2/solver_08_part2.h"
#include "09/part1/solver_09_part1.h"
#include "09/part2/solver_09_part2.h"
#include "10/part1/solver_10_part1.h"
#include "10/part2/solver_10_part2.h"
#include "11/part1/solver_11_part1.h"
#include "11/part2/solver_11_part2.h"
#include "12/part1/solver_12_part1.h"
#include "12/part2/solver_12_part2.h"
#include "13/part1/solver_13_part1.h"
#include "13/part2/solver_13_part2.h"
#include "14/part1/solver_14_part1.h"
#include "14/part2/solver_14_part2.h"
#include "15/part1/solver_15_part1.h"
#include "15/part2/solver_15_part2.h"
#include "16/part1/solver_16_part1.h"
#include "16/part2/solver_16_part2.h"
#include "17/part1/solver_17_part1.h"
#include "17/part2/solver_17_part2.h"

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

  docopt::value dayArg = args["<day>"];
  docopt::value partArg = args["<part>"];
  docopt::value inputFilePathArg = args["<file>"];

  long day = 0;
  long part = 0;
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
  } else if (day == 8) {
    if (part == 1) {
      solver = new Solver_08_part1;
    } else if (part == 2) {
      solver = new Solver_08_part2;
    }
  } else if (day == 9) {
    if (part == 1) {
      solver = new Solver_09_part1;
    } else if (part == 2) {
      solver = new Solver_09_part2;
    }
  } else if (day == 10) {
    if (part == 1) {
      solver = new Solver_10_part1;
    } else if (part == 2) {
      solver = new Solver_10_part2;
    }
  } else if (day == 11) {
    if (part == 1) {
      solver = new Solver_11_part1;
    } else if (part == 2) {
      solver = new Solver_11_part2;
    }
  } else if (day == 12) {
    if (part == 1) {
      solver = new Solver_12_part1;
    } else if (part == 2) {
      solver = new Solver_12_part2;
    }
  } else if (day == 13) {
    if (part == 1) {
      solver = new Solver_13_part1;
    } else if (part == 2) {
      solver = new Solver_13_part2;
    }
  } else if (day == 14) {
    if (part == 1) {
      solver = new Solver_14_part1;
    } else if (part == 2) {
      solver = new Solver_14_part2;
    }
  } else if (day == 15) {
    if (part == 1) {
      solver = new Solver_15_part1;
    } else if (part == 2) {
      solver = new Solver_15_part2;
    }
  } else if (day == 16) {
    if (part == 1) {
      solver = new Solver_16_part1;
    } else if (part == 2) {
      solver = new Solver_16_part2;
    }
  } else if (day == 17) {
    if (part == 1) {
      solver = new Solver_17_part1;
    } else if (part == 2) {
      solver = new Solver_17_part2;
    }
  }

  if (solver == nullptr) {
    spdlog::error("No solver for day {} part {}", day, part);
    return 1;
  }

  std::cout << "Result: " << solver->solve(inputFile) << std::endl;

  return 0;
}
