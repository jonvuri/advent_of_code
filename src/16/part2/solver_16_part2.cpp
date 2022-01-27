#include <algorithm>
#include <cctype>
#include <cstddef>
#include <functional>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

#include <doctest/doctest.h>

#include "solver_16_part2.h"


namespace {

std::string hex_to_bin(char c)
{
  if (isspace(c) != 0) {
    return "";
  }

  switch (c) {
  case '0':
    return "0000";
  case '1':
    return "0001";
  case '2':
    return "0010";
  case '3':
    return "0011";
  case '4':
    return "0100";
  case '5':
    return "0101";
  case '6':
    return "0110";
  case '7':
    return "0111";
  case '8':
    return "1000";
  case '9':
    return "1001";
  case 'A':
    return "1010";
  case 'B':
    return "1011";
  case 'C':
    return "1100";
  case 'D':
    return "1101";
  case 'E':
    return "1110";
  case 'F':
    return "1111";
  default:
    throw solver_runtime_error("Invalid hex character");
  }
}

enum PacketTypeId {
  Sum = 0,
  Product = 1,
  Minimum = 2,
  Maximum = 3,
  Literal = 4,
  GreaterThan = 5,
  LessThan = 6,
  EqualTo = 7
};

unsigned long parse_packet(std::string &bin)
{
  // Skip over unneeded version string from bin.begin() to bin.begin() + 3
  std::string type_id_str{ bin.begin() + 3, bin.begin() + 6 };
  bin.erase(bin.begin(), bin.begin() + 6);

  int type_id = std::stoi(type_id_str, nullptr, 2);

  if (type_id == Literal) {
    std::string value_str;

    bool keep_reading = true;
    while (keep_reading) {
      if (bin.front() == '0') {
        keep_reading = false;
      }
      bin.erase(bin.begin());

      value_str.append({ bin.begin(), bin.begin() + 4 });
      bin.erase(bin.begin(), bin.begin() + 4);
    }

    return std::stoul(value_str, nullptr, 2);
  } else {//Operator type
    std::vector<unsigned long> sub_packet_values;

    char length_type_id{ bin.front() };
    bin.erase(bin.begin());

    if (length_type_id == '0') {//Bit length
      std::string bit_length_str{ bin.begin(), bin.begin() + 15 };
      bin.erase(bin.begin(), bin.begin() + 15);

      unsigned long bit_length = std::stoul(bit_length_str, nullptr, 2);

      size_t curr_size = bin.size();
      while (bin.size() > curr_size - bit_length) {
        sub_packet_values.push_back(parse_packet(bin));
      }
    } else {//Packet length
      std::string packet_length_str{ bin.begin(), bin.begin() + 11 };
      bin.erase(bin.begin(), bin.begin() + 11);

      unsigned long packet_length = std::stoul(packet_length_str, nullptr, 2);

      for (unsigned long i = 0; i < packet_length; i++) {
        sub_packet_values.push_back(parse_packet(bin));
      }
    }

    if (type_id == Sum) {
      return std::accumulate(sub_packet_values.begin(), sub_packet_values.end(), 0UL);
    } else if (type_id == Product) {
      return std::accumulate(sub_packet_values.begin(), sub_packet_values.end(), 1UL, std::multiplies<>());
    } else if (type_id == Minimum) {
      return *std::min_element(sub_packet_values.begin(), sub_packet_values.end());
    } else if (type_id == Maximum) {
      return *std::max_element(sub_packet_values.begin(), sub_packet_values.end());
    } else if (type_id == GreaterThan) {
      return sub_packet_values[0] > sub_packet_values[1] ? 1 : 0;
    } else if (type_id == LessThan) {
      return sub_packet_values[0] < sub_packet_values[1] ? 1 : 0;
    } else if (type_id == EqualTo) {
      return sub_packet_values[0] == sub_packet_values[1] ? 1 : 0;
    } else {
      throw solver_runtime_error("Invalid packet type ID");
    }
  }
}

}// namespace


std::string Solver_16_part2::solve(std::istream &is)
{
  std::string binary_input_str;
  std::string line;

  is >> std::ws;
  std::getline(is, line);

  for (const char ch : line) {
    binary_input_str.append(hex_to_bin(ch));
  }

  return std::to_string(parse_packet(binary_input_str));
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
