#include <string>
#include <sstream>
#include <vector>

#include <doctest/doctest.h>

#include "solver_04_part1.h"


unsigned long Solver_04_part1::solve(std::istream &is)
{
  std::string drawInput;
  unsigned long value;
  std::vector<unsigned long> boardNumbers;

  if (!(is >> drawInput)) {
    throw "Empty or invalid input";
  }

  std::stringstream ds(drawInput);

  while (is >> value) {
    boardNumbers.push_back(value);
  }

  size_t len = boardNumbers.size();

  if (len % 25 != 0) {
    throw "Invalid number of board values - must be 25 each";
  }

  unsigned long boardCount = len / 25;
  std::vector<bool> drawnNumbers(len, false);

  std::string drawStr;
  unsigned long draw;

  // Win buffers - each row and col starts as assumed win, gets set to false if undrawn number appears
  bool rowWin[5];
  bool colWin[5];

  unsigned long winningBoard;

  while (std::getline(ds, drawStr, ',')) {
    draw = std::stoul(drawStr);

    for (unsigned long b = 0; b < boardCount; b++) {
      // Initialize buffers
      for (unsigned long i = 0; i < 5; i++) {
        rowWin[i] = true;
        colWin[i] = true;
      }

      for (unsigned long i = b * 25; i < (b + 1) * 25; i++) {
        if (boardNumbers[i] == draw) {
          drawnNumbers[i] = true;
        }

        // If not drawn now or prior, mark number's row/col as losing
        if (!drawnNumbers[i]) {
          rowWin[(i % 25) / 5] = false;
          colWin[i % 5] = false;
        }
      }

      for (unsigned long i = 0; i < 5; i++) {
        if (rowWin[i] || colWin[i]) {
          // Win!
          winningBoard = b;
          // goto to easily jump out of all loops
          goto win;
        }
      }
    }
  }

  // No winning board found after drawing all numbers. This is unexpected
  return 0;

win:
  unsigned long winningUndrawnSum = 0;

  // Now compute sum of undrawn numbers
  for (unsigned long i = winningBoard * 25; i < (winningBoard + 1) * 25; i++) {
    if (!drawnNumbers[i]) {
      winningUndrawnSum += boardNumbers[i];
    }
  }

  return draw * winningUndrawnSum;
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
