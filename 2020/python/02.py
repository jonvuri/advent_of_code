import re
from time import process_time
import unittest

import utils



def solve_part1(lines):
    valid_lines = 0

    for line in lines:
        match = re.match(r"(\d+)-(\d+) ([a-z]): ([a-z]+)", line)

        min_chars = int(match.group(1))
        max_chars = int(match.group(2))
        char = match.group(3)
        password = match.group(4)

        char_count = password.count(char)

        if char_count >= min_chars and char_count <= max_chars:
            valid_lines += 1
    
    return valid_lines



def solve_part2(lines):
    valid_lines = 0

    for line in lines:
        match = re.match(r"(\d+)-(\d+) ([a-z]): ([a-z]+)", line)

        char_pos1 = int(match.group(1)) - 1 # make 0-indexed
        char_pos2 = int(match.group(2)) - 1
        char = match.group(3)
        password = match.group(4)

        char_count = password.count(char)

        # If one and only one char matches (xor)
        if (password[char_pos1] == char) != (password[char_pos2] == char):
            valid_lines += 1
    
    return valid_lines




if __name__ == '__main__':
    start_time = process_time()

    print('Part 1: ', solve_part1(utils.load_lines('inputs/02.txt')))
    print('Part 2: ', solve_part2(utils.load_lines('inputs/02.txt')))

    print('Took {:.2} seconds'.format(process_time() - start_time))



# Tests

test_lines = """
1-3 a: abcde
1-3 b: cdefg
2-9 c: ccccccccc
"""

part1_test_solution = 2
part2_test_solution = 1

class TestSolver(unittest.TestCase):
    def test_solve_part1(self):
        self.assertEqual(solve_part1(utils.load_test_lines(test_lines)), part1_test_solution)

    def test_solve_part2(self):
        self.assertEqual(solve_part2(utils.load_test_lines(test_lines)), part2_test_solution)
