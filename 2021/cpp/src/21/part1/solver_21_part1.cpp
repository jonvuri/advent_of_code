#include <regex>
#include <sstream>
#include <string>
#include <vector>

#include <doctest/doctest.h>

#include "solver_21_part1.h"


namespace {

struct Player
{
  int number;
  int position;
  int score;

  void advance(int spaces)
  {
    position = ((position + spaces - 1) % 10) + 1;
    score += position;
  }
};

Player
  get_next_player(std::istream &is)
{
  std::string input;
  std::getline(is >> std::ws, input);

  std::smatch match_results;
  bool match = std::regex_search(input, match_results, std::regex(R"(Player (\d+) starting position: (\d+))"), std::regex_constants::match_default);
  if (!match) {
    throw solver_runtime_error("Invalid input");
  }

  int number = std::stoi(match_results[1]);
  int position = std::stoi(match_results[2]);

  return Player{ number, position, 0 };
}

struct Die
{
  int rolls = 0;
  int value = 1;

  int roll()
  {
    rolls++;
    if (value > 100) {
      value = 1;
    }
    return value++;
  }
};

}// namespace


std::string Solver_21_part1::solve(std::istream &is)
{
  Player player1 = get_next_player(is);
  Player player2 = get_next_player(is);

  Die die;

  int losing_score = 0;

  while (true) {
    player1.advance(die.roll() + die.roll() + die.roll());
    if (player1.score >= 1000) {
      losing_score = player2.score;
      break;
    }


    player2.advance(die.roll() + die.roll() + die.roll());
    if (player2.score >= 1000) {
      losing_score = player1.score;
      break;
    }
  }

  return std::to_string(losing_score * die.rolls);
}

TEST_CASE("testing solver for day 21 part 1 - dice game")
{
  Solver_21_part1 solver;

  {
    std::istringstream is(std::string{ R"(
      Player 1 starting position: 4
      Player 2 starting position: 8
    )" });

    CHECK(solver.solve(is) == "739785");
  }
}
