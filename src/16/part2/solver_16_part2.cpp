#include <algorithm>
#include <array>
#include <bitset>
#include <limits>
#include <numeric>
#include <stack>
#include <string>
#include <sstream>
#include <tuple>
#include <vector>

#include <doctest/doctest.h>

#include "solver_16_part2.h"


namespace {

unsigned long read_number(const std::string &bit_list)
{
  std::bitset<32> bits(bit_list);
  return bits.to_ulong();
}

enum Operation {
  Sum = 0,
  Product = 1,
  Minimum = 2,
  Maximum = 3,
  GreaterThan = 5,
  LessThan = 6,
  EqualTo = 7
};

unsigned long
  perform_operation(Operation type, std::vector<unsigned long> values)
{
  if (type < 4) {// List operations
    switch (type) {
    case Sum:
      return std::accumulate(values.begin(), values.end(), 0UL);
    case Product:
      return std::accumulate(values.begin(), values.end(), 1UL, std::multiplies<>());
    case Minimum:
      return *std::min_element(values.begin(), values.end());
    case Maximum:
      return *std::max_element(values.begin(), values.end());
    default:
      throw solver_runtime_error("Unexpected operation type ID");
    }
  } else {// Binary operations
    if (values.size() != 2) {
      throw solver_runtime_error("Didn't get two values for binary operation");
    }

    switch (type) {
    case GreaterThan:
      return values[0] > values[1] ? 1 : 0;
    case LessThan:
      return values[0] < values[1] ? 1 : 0;
    case EqualTo:
      return values[0] == values[1] ? 1 : 0;
    default:
      throw solver_runtime_error("Unexpected operation type ID");
    }
  }
}

// Returns tuple of pos after reading, and value of packet [value, pos]
std::tuple<unsigned long, unsigned int> read_packet(std::string input, unsigned int pos)
{
  if (pos >= input.size()) {
    // No more packets
    return { 0, pos };
  }

  pos += 3;
  const auto type_id = read_number(input.substr(pos, 3));
  pos += 3;

  if (type_id == 4) {// Literal
    std::string literal;

    for (bool go = true; go; pos += 5) {
      go = input[pos] == '1';
      literal += input.substr(pos + 1, 4);
    }

    return { read_number(literal), pos };
  } else {// Operator
    const auto length_bit = input[pos];
    pos += 1;

    std::vector<unsigned long> packet_values;

    if (length_bit == '0') {// Length given in number of bits in subpackets
      const auto total_bits = read_number(input.substr(pos, 15));
      pos += 15;

      const auto end_bit = pos + total_bits;

      while (pos < end_bit) {
        auto [value, packet_end_pos] = read_packet(input, pos);
        pos = packet_end_pos;

        packet_values.push_back(value);
      }
    } else {// Length given in number of subpackets
      const auto total_packets = read_number(input.substr(pos, 11));
      pos += 11;

      for (unsigned int p = 0; p < total_packets; ++p) {
        auto [value, packet_end_pos] = read_packet(input, pos);
        pos = packet_end_pos;

        packet_values.push_back(value);
      }
    }

    return { perform_operation(static_cast<Operation>(type_id), packet_values), pos };
  }
}

}// namespace


std::string Solver_16_part2::solve(std::istream &is)
{
  std::string binary_input;

  for (char c = 0; is.get(c);) {
    if (isspace(c) == 0) {
      unsigned long num = 0;

      std::istringstream iss(std::string(1, c));
      iss >> std::hex >> num;

      std::bitset<4> bit_input(num);

      binary_input.append(bit_input.to_string());
    }
  }

  auto [value, _] = read_packet(binary_input, 0);

  return std::to_string(value);
}

TEST_CASE("testing solver for day 16 part 2 - hexadecimal packet parser with operators implemented")
{
  Solver_16_part2 solver;

  {
    std::istringstream is(std::string{ R"(
      C200B40A82
    )" });

    CHECK(solver.solve(is) == "3");
  }

  {
    std::istringstream is(std::string{ R"(
      04005AC33890
    )" });

    CHECK(solver.solve(is) == "54");
  }

  {
    std::istringstream is(std::string{ R"(
      880086C3E88112
    )" });

    CHECK(solver.solve(is) == "7");
  }

  {
    std::istringstream is(std::string{ R"(
      CE00C43D881120
    )" });

    CHECK(solver.solve(is) == "9");
  }

  {
    std::istringstream is(std::string{ R"(
      D8005AC2A8F0
    )" });

    CHECK(solver.solve(is) == "1");
  }

  {
    std::istringstream is(std::string{ R"(
      F600BC2D8F
    )" });

    CHECK(solver.solve(is) == "0");
  }

  {
    std::istringstream is(std::string{ R"(
      9C005AC2F8F0
    )" });

    CHECK(solver.solve(is) == "0");
  }

  {
    std::istringstream is(std::string{ R"(
      9C0141080250320F1802104A08
    )" });

    CHECK(solver.solve(is) == "1");
  }
}
