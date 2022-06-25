from time import process_time
import unittest

import utils



def solve_part1(lines):
    inputs = list(map(int, lines))

    for i in range(len(inputs)):
        a = inputs[i]

        for b in inputs[i:]:
            if (a + b) == 2020:
                return a * b



def solve_part2(lines):
    inputs = list(map(int, lines))

    for i in range(len(inputs)):
        a = inputs[i]

        for j in range(i + 1, len(inputs)):
            b = inputs[j]

            for c in inputs[j:]:
                if (a + b + c) == 2020:
                    return a * b * c



if __name__ == '__main__':
    start_time = process_time()

    print('Part 1: ', solve_part1(utils.load_lines('inputs/01.txt')))
    print('Part 2: ', solve_part2(utils.load_lines('inputs/01.txt')))

    print('Took {:.2} seconds'.format(process_time() - start_time))



# Tests

test_lines = """
1721
979
366
299
675
1456
"""

part1_test_solution = 514579
part2_test_solution = 241861950

class TestSolver(unittest.TestCase):
    def test_solve_part1(self):
        self.assertEqual(solve_part1(utils.load_test_lines(test_lines)), part1_test_solution)

    def test_solve_part2(self):
        self.assertEqual(solve_part2(utils.load_test_lines(test_lines)), part2_test_solution)
