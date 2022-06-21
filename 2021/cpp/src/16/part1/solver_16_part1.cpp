#include <bitset>
#include <cctype>
#include <sstream>
#include <string>
#include <tuple>

#include <doctest/doctest.h>

#include "solver_16_part1.h"


namespace {

unsigned long read_number(const std::string &bit_list)
{
  std::bitset<32> bits(bit_list);
  return bits.to_ulong();
}

// Returns tuple of pos after reading, and value of packet [sum, pos]
std::tuple<unsigned long, unsigned int> read_packet(std::string input, unsigned int pos)
{
  unsigned long version_sum = 0;

  if (pos >= input.size()) {
    // No more packets
    return { version_sum, pos };
  }

  const auto version = read_number(input.substr(pos, 3));
  pos += 3;
  const auto type_id = read_number(input.substr(pos, 3));
  pos += 3;

  version_sum += version;

  if (type_id == 4) {// Literal
    // Chomp literal value, unneeded
    for (bool go = true; go; pos += 5) {
      go = input[pos] == '1';
    }
  } else {// Operator
    const auto length_bit = input[pos];
    pos += 1;

    if (length_bit == '0') {// Length given in number of bits in subpackets
      const auto total_bits = read_number(input.substr(pos, 15));
      pos += 15;

      const auto end_bit = pos + total_bits;

      while (pos < end_bit) {
        auto [packet_sum, packet_end_pos] = read_packet(input, pos);
        version_sum += packet_sum;
        pos = packet_end_pos;
      }
    } else {// Length given in number of subpackets
      const auto total_packets = read_number(input.substr(pos, 11));
      pos += 11;

      for (unsigned int p = 0; p < total_packets; ++p) {
        auto [packet_sum, packet_end_pos] = read_packet(input, pos);
        version_sum += packet_sum;
        pos = packet_end_pos;
      }
    }
  }

  return { version_sum, pos };
}

}// namespace


std::string Solver_16_part1::solve(std::istream &is)
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

  auto [version_sum, _] = read_packet(binary_input, 0);

  return std::to_string(version_sum);
}

TEST_CASE("testing solver for day 16 part 1 - hexadecimal packet parser")
{
  Solver_16_part1 solver;

  {
    std::istringstream is(std::string{ R"(
      8A004A801A8002F478
    )" });

    CHECK(solver.solve(is) == "16");
  }

  {
    std::istringstream is(std::string{ R"(
      620080001611562C8802118E34
    )" });

    CHECK(solver.solve(is) == "12");
  }

  {
    std::istringstream is(std::string{ R"(
      C0015000016115A2E0802F182340
    )" });

    CHECK(solver.solve(is) == "23");
  }

  {
    std::istringstream is(std::string{ R"(
      A0016C880162017C3686B18A3D4780
    )" });

    CHECK(solver.solve(is) == "31");
  }
}
