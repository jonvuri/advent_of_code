#include <array>
#include <climits>
#include <cstdint>
#include <cstdlib>
#include <queue>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

#include <doctest/doctest.h>

#include "solver_21_part2.h"


namespace {

char get_next_initial_position(std::istream &is)
{
  std::string input;
  std::getline(is >> std::ws, input);

  std::smatch match_results;
  bool match = std::regex_search(input, match_results, std::regex(R"(Player \d+ starting position: (\d+))"), std::regex_constants::match_default);
  if (!match) {
    throw solver_runtime_error("Invalid input");
  }

  int initial_position = std::stoi(match_results[1]);

  if (initial_position < SCHAR_MIN || initial_position > SCHAR_MAX) {
    throw solver_runtime_error("Input out of range");
  }

  return static_cast<char>(initial_position);
}

struct Roll
{
  char wins;// Number of wins if this value reaches 21 / number of dice permutations
  char position;// Total position increment after 3 dice rolls
};

std::array<Roll, 7> rolls = {
  Roll{ 1, 3 },
  Roll{ 3, 4 },
  Roll{ 6, 5 },
  Roll{ 7, 6 },
  Roll{ 6, 7 },
  Roll{ 3, 8 },
  Roll{ 1, 9 }
};

void roll_dice(char position_A, char score_A, int_fast64_t *wins_A, char position_B, char score_B, int_fast64_t *wins_B, int_fast64_t score_mult)
{
  if (score_B >= 21) {
    (*wins_B) += score_mult;
  } else {
    for (const auto &roll : rolls) {
      char new_position_A = static_cast<char>(((position_A + roll.position - 1) % 10) + 1);
      char new_score_A = static_cast<char>(score_A + new_position_A);
      roll_dice(position_B, score_B, wins_B, new_position_A, new_score_A, wins_A, score_mult * roll.wins);
    }
  }
}

}// namespace


std::string
  Solver_21_part2::solve(std::istream &is)
{
  auto p1_initial_position = get_next_initial_position(is);
  auto p2_initial_position = get_next_initial_position(is);

  int_fast64_t p1_wins = 0;
  int_fast64_t p2_wins = 0;

  roll_dice(p1_initial_position, 0, &p1_wins, p2_initial_position, 0, &p2_wins, 1);

  return std::to_string(std::max(p1_wins, p2_wins));
}

TEST_CASE("testing solver for day 21 part 2 - quantum dice game")
{
  Solver_21_part2 solver;

  {
    std::istringstream is(std::string{ R"(
      Player 1 starting position: 4
      Player 2 starting position: 8
    )" });

    CHECK(solver.solve(is) == "444356092776315");
  }
}
