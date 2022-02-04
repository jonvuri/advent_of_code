#include <algorithm>
#include <bitset>
#include <cstddef>
#include <numeric>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include <doctest/doctest.h>

#include "solver_20_part1.h"


namespace {

std::vector<std::vector<bool>> enhance_canvas(
  const std::vector<std::vector<bool>> &input_canvas,
  const std::vector<bool> &algorithm,
  bool even_iteration// If this is true and algorithm[0] is true, the infinite grid surrounding the image will be lit
)
{
  if (input_canvas.empty()) {
    return input_canvas;
  }

  const auto input_canvas_width = input_canvas[0].size();
  const auto input_canvas_height = input_canvas.size();

  const auto canvas_width = input_canvas_width + 2;
  const auto canvas_height = input_canvas_height + 2;

  std::vector<std::vector<bool>> canvas{};

  std::bitset<9> pixel_region;

  for (size_t y = 0; y < canvas_height; ++y) {
    std::vector<bool> row(canvas_width);

    for (size_t x = 0; x < canvas_width; ++x) {
      // Fill bitset with pixel values from surrounding region
      size_t bit = 8;

      for (long ry = -1; ry <= 1; ++ry) {
        for (long rx = -1; rx <= 1; ++rx) {
          const long ix = (static_cast<long>(x) + rx) - 1;// Offset by -1 to get input canvas coordinate
          const long iy = (static_cast<long>(y) + ry) - 1;// Offset by -1 to get input canvas coordinate

          if (ix < 0 or iy < 0) {
            // 'Infinite grid' pixel outside input canvas bounds
            pixel_region[bit] = even_iteration && algorithm.at(0);
          } else {
            // Mapped back to input canvas
            const auto ax = static_cast<size_t>(ix);
            const auto ay = static_cast<size_t>(iy);

            if (ax >= input_canvas_width or ay >= input_canvas_height) {
              // 'Infinite grid' pixel outside bounds
              pixel_region[bit] = even_iteration && algorithm.at(0);
            } else {
              pixel_region[bit] = input_canvas.at(ay).at(ax);
            }
          }

          bit -= 1;
        }
      }

      bool output_pixel = algorithm.at(pixel_region.to_ulong());
      row.at(x) = output_pixel;
    }

    canvas.push_back(std::move(row));
  }

  return canvas;
}

long count_lit_pixels(const std::vector<std::vector<bool>> &canvas)
{
  return std::accumulate(
    canvas.begin(),
    canvas.end(),
    0,
    [](long sum, const auto &row) { return sum + std::count(row.begin(), row.end(), true); });
}

}// namespace


std::string Solver_20_part1::solve(std::istream &is)
{
  // Ingest algorithm inputs
  std::vector<bool> algorithm_inputs;
  {
    is >> std::ws;

    std::string line;
    std::getline(is, line);
    std::stringstream ss(line);

    char input = 0;
    while (ss >> input) {
      if (input == '.') {
        algorithm_inputs.push_back(false);
      } else if (input == '#') {
        algorithm_inputs.push_back(true);
      } else {
        throw solver_runtime_error("Invalid algorithm input");
      }
    }
  }

  // Ingest image inputs
  std::vector<std::vector<bool>> image_inputs = {};
  {
    is >> std::ws;

    image_inputs.emplace_back(std::vector<bool>());

    size_t x = 0;
    size_t y = 0;

    char input = 0;
    while (is.get(input)) {
      if (input == ' ') {
        continue;
      }

      if (input == '\n') {
        if (x != 0) {
          y += 1;
          image_inputs.emplace_back(std::vector<bool>());
        }
        x = 0;
      } else {
        if (input == '.') {
          image_inputs.at(y).push_back(false);
        } else if (input == '#') {
          image_inputs.at(y).push_back(true);
        } else {
          throw solver_runtime_error("Invalid image input");
        }
        x += 1;
      }
    }
    if (image_inputs.back().empty()) {
      image_inputs.pop_back();
    }
  }

  int iteration = 1;
  std::vector<std::vector<bool>> first_output = enhance_canvas(image_inputs, algorithm_inputs, iteration % 2 == 0);
  iteration += 1;
  std::vector<std::vector<bool>> second_output = enhance_canvas(first_output, algorithm_inputs, iteration % 2 == 0);

  return std::to_string(count_lit_pixels(second_output));
}

TEST_CASE("testing solver for day 20 part 1 - image enhancement")
{
  Solver_20_part1 solver;

  std::istringstream is(std::string{ R"(
    ..#.#..#####.#.#.#.###.##.....###.##.#..###.####..#####..#....#..#..##..###..######.###...####..#..#####..##..#.#####...##.#.#..#.##..#.#......#.###.######.###.####...#.##.##..#..#..#####.....#.#....###..#.##......#.....#..#..#..##..#...##.######.####.####.#.#...#.......#..#.#.#...####.##.#......#..#...##.#.##..#...##.#.##..###.#......#.#.......#.#.#.####.###.##...#.....####.#..#..#.##.#....##..#.####....##...##..#...#......#.#.......#.......##..####..#...#.#.#...##..#.#..###..#####........#..####......#..#

    #..#.
    #....
    ##..#
    ..#..
    ..###
  )" });

  CHECK(solver.solve(is) == "35");
}
