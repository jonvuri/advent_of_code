#include <bitset>
#include <cstdlib>
#include <optional>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

#include <doctest/doctest.h>

#include "solver_22_part1.h"


namespace {

const int MATRIX_MIN = -50;
const size_t MATRIX_SIZE = 100;

typedef std::bitset<MATRIX_SIZE * MATRIX_SIZE * MATRIX_SIZE> Matrix;

struct Instruction
{
  bool turn_on;
  size_t x_min;
  size_t x_max;
  size_t y_min;
  size_t y_max;
  size_t z_min;
  size_t z_max;
};

std::optional<Instruction> get_next_instruction(std::istream &is)
{
  std::string input;
  std::getline(is >> std::ws, input);

  std::smatch match_results;
  bool match = std::regex_search(input, match_results, std::regex(R"((on|off) x=([\d-]+)\.\.([\d-]+),y=([\d-]+)\.\.([\d-]+),z=([\d-]+)\.\.([\d-]+))"), std::regex_constants::match_default);

  if (!match) {
    return {};
  }

  bool turn_on = match_results[1] == "on";
  int x_min = std::stoi(match_results[2]) - MATRIX_MIN;
  int x_max = std::stoi(match_results[3]) - MATRIX_MIN;
  int y_min = std::stoi(match_results[4]) - MATRIX_MIN;
  int y_max = std::stoi(match_results[5]) - MATRIX_MIN;
  int z_min = std::stoi(match_results[6]) - MATRIX_MIN;
  int z_max = std::stoi(match_results[7]) - MATRIX_MIN;

  if (x_min < 0 or x_max < 0 or y_min < 0 or y_max < 0 or z_min < 0 or z_max < 0) {
    // Instruction out of range, ignore
    return {};
  }

  auto x_min_size = static_cast<size_t>(x_min);
  auto x_max_size = static_cast<size_t>(x_max);
  auto y_min_size = static_cast<size_t>(y_min);
  auto y_max_size = static_cast<size_t>(y_max);
  auto z_min_size = static_cast<size_t>(z_min);
  auto z_max_size = static_cast<size_t>(z_max);

  if (x_max_size > MATRIX_SIZE or y_max_size > MATRIX_SIZE or z_max_size > MATRIX_SIZE) {
    // Instruction out of range, ignore
    return {};
  }

  return Instruction{
    turn_on,
    x_min_size,
    x_max_size,
    y_min_size,
    y_max_size,
    z_min_size,
    z_max_size
  };
}

void execute_instruction(Matrix &matrix, Instruction instruction)
{
  for (size_t x = instruction.x_min; x <= instruction.x_max; ++x) {
    for (size_t y = instruction.y_min; y <= instruction.y_max; ++y) {
      for (size_t z = instruction.z_min; z <= instruction.z_max; ++z) {
        matrix[x + y * MATRIX_SIZE + z * MATRIX_SIZE * MATRIX_SIZE] = instruction.turn_on;
      }
    }
  }
}

}// namespace


std::string Solver_22_part1::solve(std::istream &is)
{
  Matrix matrix;

  while (auto instruction = get_next_instruction(is)) {
    execute_instruction(matrix, *instruction);
  }

  return std::to_string(matrix.count());
}

TEST_CASE("testing solver for day 22 part 1 - 3D reactor reboot")
{
  Solver_22_part1 solver;

  {
    std::istringstream is(std::string{ R"(
      on x=-20..26,y=-36..17,z=-47..7
      on x=-20..33,y=-21..23,z=-26..28
      on x=-22..28,y=-29..23,z=-38..16
      on x=-46..7,y=-6..46,z=-50..-1
      on x=-49..1,y=-3..46,z=-24..28
      on x=2..47,y=-22..22,z=-23..27
      on x=-27..23,y=-28..26,z=-21..29
      on x=-39..5,y=-6..47,z=-3..44
      on x=-30..21,y=-8..43,z=-13..34
      on x=-22..26,y=-27..20,z=-29..19
      off x=-48..-32,y=26..41,z=-47..-37
      on x=-12..35,y=6..50,z=-50..-2
      off x=-48..-32,y=-32..-16,z=-15..-5
      on x=-18..26,y=-33..15,z=-7..46
      off x=-40..-22,y=-38..-28,z=23..41
      on x=-16..35,y=-41..10,z=-47..6
      off x=-32..-23,y=11..30,z=-14..3
      on x=-49..-5,y=-3..45,z=-29..18
      off x=18..30,y=-20..-8,z=-3..13
      on x=-41..9,y=-7..43,z=-33..15
      on x=-54112..-39298,y=-85059..-49293,z=-27449..7877
      on x=967..23432,y=45373..81175,z=27513..53682
    )" });

    CHECK(solver.solve(is) == "590784");
  }
}
