#include <array>
#include <string>
#include <sstream>
#include <vector>

#include <doctest/doctest.h>

#include "solver_04_part1.h"


static const unsigned long BOARD_WIDTH = 5;
static const unsigned long BOARD_SIZE = BOARD_WIDTH * BOARD_WIDTH;


unsigned long Solver_04_part1::solve(std::istream &is)
{
  std::string drawInput;
  unsigned long value = 0;
  std::vector<unsigned long> boardNumbers;

  if (!(is >> drawInput)) {
    throw solver_runtime_error("Empty or invalid input");
  }

  std::stringstream ds(drawInput);

  while (is >> value) {
    boardNumbers.push_back(value);
  }

  size_t len = boardNumbers.size();

  if (len % BOARD_SIZE != 0) {
    throw solver_runtime_error("Invalid number of board values - must be " + std::to_string(BOARD_SIZE) + " each");
  }

  unsigned long boardCount = len / BOARD_SIZE;
  std::vector<bool> drawnNumbers(len, false);

  // Win buffers - each row and col starts as assumed win, gets set to false if undrawn number appears
  std::array<bool, BOARD_WIDTH> rowWin = {};
  std::array<bool, BOARD_WIDTH> colWin = {};

  std::string drawStr;
  while (std::getline(ds, drawStr, ',')) {
    unsigned long draw = std::stoul(drawStr);

    for (unsigned long b = 0; b < boardCount; b++) {
      // Initialize buffers
      for (unsigned long i = 0; i < BOARD_WIDTH; i++) {
        rowWin.at(i) = true;
        colWin.at(i) = true;
      }

      for (unsigned long i = b * BOARD_SIZE; i < (b + 1) * BOARD_SIZE; i++) {
        if (boardNumbers[i] == draw) {
          drawnNumbers[i] = true;
        }

        // If not drawn now or prior, mark number's row/col as losing
        if (!drawnNumbers[i]) {
          rowWin.at((i % BOARD_SIZE) / BOARD_WIDTH) = false;
          colWin.at(i % BOARD_WIDTH) = false;
        }
      }

      for (unsigned long i = 0; i < BOARD_WIDTH; i++) {
        if (rowWin.at(i) || colWin.at(i)) {
          // Win! we're done. Now just compute undrawn sum
          unsigned long winningUndrawnSum = 0;
          for (unsigned long j = b * BOARD_SIZE; j < (b + 1) * BOARD_SIZE; j++) {
            if (!drawnNumbers[j]) {
              winningUndrawnSum += boardNumbers[j];
            }
          }

          return draw * winningUndrawnSum;
        }
      }
    }
  }

  // No winning board found after drawing all numbers. This is unexpected
  return 0;
}

TEST_CASE("testing solver for day 4 part 1 - bingo game")
{
  Solver_04_part1 solver;
  std::istringstream is(std::string{ R"(
    7,4,9,5,11,17,23,2,0,14,21,24,10,16,13,6,15,25,12,22,18,20,8,19,3,26,1

    22 13 17 11  0
    8  2 23  4 24
    21  9 14 16  7
    6 10  3 18  5
    1 12 20 15 19

    3 15  0  2 22
    9 18 13 17  5
    19  8  7 25 23
    20 11 10 24  4
    14 21 16 12  6

    14 21 17 24  4
    10 16 15  9 19
    18  8 23 26 20
    22 11 13  6  5
    2  0 12  3  7
  )" });

  CHECK(solver.solve(is) == 4512);
}
