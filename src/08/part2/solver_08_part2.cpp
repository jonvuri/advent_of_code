#include <algorithm>
#include <array>
#include <cmath>
#include <limits>
#include <map>
#include <set>
#include <string>
#include <sstream>
#include <vector>

#include <doctest/doctest.h>

#include "solver_08_part2.h"


static unsigned long quick_pow10(const unsigned long &n)
{
  static const std::array<unsigned long, 10> pow10 = {
    1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000
  };

  return pow10.at(n);
}


unsigned long
  Solver_08_part2::solve(std::istream &is)
{
  unsigned long output_sum = 0;

  std::string line;
  while (std::getline(is, line)) {
    std::istringstream iss(line);

    std::vector<std::string> test_digits;
    std::vector<std::string> output_digits;

    std::string input;
    while (iss >> input) {
      if (input[0] == '|') {
        break;
      }
      test_digits.push_back(input);
    }
    while (iss >> input) {
      output_digits.push_back(input);
    }

    if (test_digits.empty() || output_digits.empty()) {
      continue;
    }

    // Decoded characters as we discover them (referred to as a', b', etc. later)
    char a = 0;
    char b = 0;
    char c = 0;
    char d = 0;
    char e = 0;
    char f = 0;
    char g = 0;

    // First count the occurrences of each character in the input digits.
    // Since this will match the amount of each original character, and
    // we know the unordered decoded output (all ten digits, once each),
    // this lets us determine a few character decodings uniquely:
    //
    // 0 -> [a b c e f g]
    // 1 -> [c f]
    // 2 -> [a c d e g]
    // 3 -> [a c d f g]
    // 4 -> [b c d f]
    // 5 -> [a b d f g]
    // 6 -> [a b d e f g]
    // 7 -> [a c f]
    // 8 -> [a b c d e f g]
    // 9 -> [a b c d f g]
    //
    // Occurrences of each (original, unencoded) character:
    // a -> 8
    // b -> 6 *
    // c -> 8
    // d -> 7
    // e -> 4 *
    // f -> 9 *
    // g -> 7
    //
    // b, e, and f have unique counts, so we can determine them this way
    //
    std::map<char, unsigned int> char_counts = {
      { 'a', 0 },
      { 'b', 0 },
      { 'c', 0 },
      { 'd', 0 },
      { 'e', 0 },
      { 'f', 0 },
      { 'g', 0 }
    };

    for (const auto &digit : test_digits) {
      for (const char &ch : digit) {
        char_counts.insert_or_assign(ch, char_counts.at(ch) + 1);
      }
    }

    for (const auto &[ch, count] : char_counts) {
      switch (count) {
      case 4:// This is e'
        e = ch;
        break;
      case 6:// This is b'
        b = ch;
        break;
      case 9:// This is f'
        f = ch;
        break;
      default:
        break;
      }
    }


    // Now, 1, 4, and 7 digits have unique lengths, so we can find them and use
    // their character sets as bounding sets for each included character.
    // Removing characters we already know can determine others uniquely.
    // 1 -> [c' f']
    // 4 -> [b' c' d' f']
    // 7 -> [a' c' f']

    // Find the 1 digit and get c'
    const auto &digit1 = *std::find_if(test_digits.begin(), test_digits.end(), [](const auto &str) -> bool { return str.length() == 2; });
    c = digit1[0] == f ? digit1[1] : digit1[0];

    // Now that c' is known, we can narrow the other two down to a single character too:

    // Find the 4 digit and get d'
    const auto &digit4 = *std::find_if(test_digits.begin(), test_digits.end(), [](const auto &str) -> bool { return str.length() == 4; });
    d = *std::find_if(digit4.begin(), digit4.end(), [&](const auto &ch) -> bool { return ch != b && ch != c && ch != f; });


    // Find the 7 digit and get a'
    const auto &digit7 = *std::find_if(test_digits.begin(), test_digits.end(), [](const auto &str) -> bool { return str.length() == 3; });
    a = *std::find_if(digit7.begin(), digit7.end(), [&](const auto &ch) -> bool { return ch != c && ch != f; });


    // Only g' remains, so simple elimination:
    const auto all_chars = { 'a', 'b', 'c', 'd', 'e', 'f', 'g' };
    g = *std::find_if(all_chars.begin(), all_chars.end(), [&](const auto &ch) -> bool { return ch != a && ch != b && ch != c && ch != d && ch != e && ch != f; });


    // We now have all characters decoded, so we can build decoder sets for each digit:
    const std::vector<std::pair<unsigned long, std::set<char>>> decoder_sets = {
      { 0, { a, b, c, e, f, g } },
      { 1, { c, f } },
      { 2, { a, c, d, e, g } },
      { 3, { a, c, d, f, g } },
      { 4, { b, c, d, f } },
      { 5, { a, b, d, f, g } },
      { 6, { a, b, d, e, f, g } },
      { 7, { a, c, f } },
      { 8, { a, b, c, d, e, f, g } },
      { 9, { a, b, c, d, f, g } }
    };


    // Finally, compute the output sum for this entry
    for (unsigned long i = 0; i < output_digits.size(); ++i) {
      std::set<char> output_digit_set;
      for (const char &ch : output_digits[i]) {
        output_digit_set.insert(ch);
      }

      for (const auto &[digit, digit_set] : decoder_sets) {
        if (output_digit_set == digit_set) {
          output_sum += digit * quick_pow10(output_digits.size() - i - 1);
          break;
        }
      }
    }
  }

  return output_sum;
}

TEST_CASE("testing solver for day 8 part 2 - seven segment display full decoding with output sum")
{
  Solver_08_part2 solver;
  std::istringstream is(std::string{ R"(
    be cfbegad cbdgef fgaecd cgeb fdcge agebfd fecdb fabcd edb | fdgacbe cefdb cefbgd gcbe
    edbfga begcd cbg gc gcadebf fbgde acbgfd abcde gfcbed gfec | fcgedb cgb dgebacf gc
    fgaebd cg bdaec gdafb agbcfd gdcbef bgcad gfac gcb cdgabef | cg cg fdcagb cbg
    fbegcd cbd adcefb dageb afcb bc aefdc ecdab fgdeca fcdbega | efabcd cedba gadfec cb
    aecbfdg fbg gf bafeg dbefa fcge gcbea fcaegb dgceab fcbdga | gecf egdcabf bgf bfgea
    fgeab ca afcebg bdacfeg cfaedg gcfdb baec bfadeg bafgc acf | gebdcfa ecba ca fadegcb
    dbcfg fgd bdegcaf fgec aegbdf ecdfab fbedc dacgb gdcebf gf | cefg dcbef fcge gbcadfe
    bdfegc cbegaf gecbf dfcage bdacg ed bedf ced adcbefg gebcd | ed bcgafe cdgba cbgef
    egadfb cdbfeg cegd fecab cgb gbdefca cg fgcdab egfdb bfceg | gbdfcae bgc cg cgb
    gcafb gcf dcaebfg ecagb gf abcdeg gaef cafbge fdbac fegbdc | fgae cfgab fg bagce
  )" });

  CHECK(solver.solve(is) == 61229);
}
