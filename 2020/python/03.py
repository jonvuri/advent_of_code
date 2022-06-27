import re
from time import process_time
import unittest

import utils



def check(lines, right, down):
    width = len(lines[0])

    x = right
    y = down

    trees = 0

    while y < len(lines):
        if lines[y][x % width] == '#':
            trees += 1

        x += right
        y += down

    return trees



def solve_part1(lines):
    return check(lines, 3, 1)


def solve_part2(lines):
    result = 1
    result *= check(lines, 1, 1)
    result *= check(lines, 3, 1)
    result *= check(lines, 5, 1)
    result *= check(lines, 7, 1)
    result *= check(lines, 1, 2)
    return result



if __name__ == '__main__':
    start_time = process_time()

    print('Part 1: ', solve_part1(utils.load_lines('inputs/03.txt')))
    print('Part 2: ', solve_part2(utils.load_lines('inputs/03.txt')))

    print('Took {:.2} seconds'.format(process_time() - start_time))



# Tests

test_lines = """
..##.......
#...#...#..
.#....#..#.
..#.#...#.#
.#...##..#.
..#.##.....
.#.#.#....#
.#........#
#.##...#...
#...##....#
.#..#...#.#
"""

part1_test_solution = 7
part2_test_solution = 336

class TestSolver(unittest.TestCase):
    def test_solve_part1(self):
        self.assertEqual(solve_part1(utils.load_test_lines(test_lines)), part1_test_solution)

    def test_solve_part2(self):
        self.assertEqual(solve_part2(utils.load_test_lines(test_lines)), part2_test_solution)
