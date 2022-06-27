import re
from time import process_time
import unittest

import utils



required_fields = ['byr', 'iyr', 'eyr', 'hgt', 'hcl', 'ecl', 'pid']


def byr_validate(value):
    return re.match('^\d{4}$', value) and int(value) >= 1920 and int(value) <= 2002

def iyr_validate(value):
    return re.match('^\d{4}$', value) and int(value) >= 2010 and int(value) <= 2020

def eyr_validate(value):
    return re.match('^\d{4}$', value) and int(value) >= 2020 and int(value) <= 2030

def hgt_validate(value):
    match = re.match('^(\d+)(cm|in)$', value)

    if not match:
        return False
    
    height = int(match.group(1))
    unit = match.group(2)

    if unit == 'cm':
        return height >= 150 and height <= 193
    else:
        return height >= 59 and height <= 76

def hcl_validate(value):
    return re.match('^#[0-9a-f]{6}$', value)

def ecl_validate(value):
    return re.match('^(amb|blu|brn|gry|grn|hzl|oth)$', value)

def pid_validate(value):
    return re.match('^\d{9}$', value)


field_validation = {
    'byr': byr_validate,
    'iyr': iyr_validate,
    'eyr': eyr_validate,
    'hgt': hgt_validate,
    'hcl': hcl_validate,
    'ecl': ecl_validate,
    'pid': pid_validate
}



def solve(lines, validate):
    passports = []
    current_passport = {}

    for line in lines:
        if line:
            # Add this line's fields to passport
            for entry in line.split(' '):
                field, value = entry.split(':')
                current_passport[field] = value
        else:
            # Blank line - add this fully collected passport and reset
            passports.append(current_passport)
            current_passport = {}

    # Might be no blank line at end of input
    passports.append(current_passport)
    
    valid_passports = 0

    for passport in passports:
        if all(map(validate(passport), required_fields)):
            valid_passports += 1

    return valid_passports


def solve_part1(lines):
    return solve(lines, lambda passport: lambda field: field in passport)

def solve_part2(lines):
    return solve(lines, lambda passport: lambda field: field in passport and field_validation[field](passport[field]))



if __name__ == '__main__':
    start_time = process_time()

    print('Part 1: ', solve_part1(utils.load_lines('inputs/04.txt', False)))
    print('Part 2: ', solve_part2(utils.load_lines('inputs/04.txt', False)))

    print('Took {:.2} seconds'.format(process_time() - start_time))



# Tests

test_lines = """
ecl:gry pid:860033327 eyr:2020 hcl:#fffffd
byr:1937 iyr:2017 cid:147 hgt:183cm

iyr:2013 ecl:amb cid:350 eyr:2023 pid:028048884
hcl:#cfa07d byr:1929

hcl:#ae17e1 iyr:2013
eyr:2024
ecl:brn pid:760753108 byr:1931
hgt:179cm

hcl:#cfa07d eyr:2025 pid:166559648
iyr:2011 ecl:brn hgt:59in
"""

invalid_test_lines = """
eyr:1972 cid:100
hcl:#18171d ecl:amb hgt:170 pid:186cm iyr:2018 byr:1926

iyr:2019
hcl:#602927 eyr:1967 hgt:170cm
ecl:grn pid:012533040 byr:1946

hcl:dab227 iyr:2012
ecl:brn hgt:182cm pid:021572410 eyr:2020 byr:1992 cid:277

hgt:59cm ecl:zzz
eyr:2038 hcl:74454a iyr:2023
pid:3556412378 byr:2007
"""

part1_test_solution = 2
part2_test_solution = 2

class TestSolver(unittest.TestCase):
    def test_solve_part1(self):
        self.assertEqual(solve_part1(utils.load_test_lines(test_lines, False)), part1_test_solution)

    def test_solve_part2(self):
        self.assertEqual(solve_part2(utils.load_test_lines(test_lines, False)), part2_test_solution)
        self.assertEqual(solve_part2(utils.load_test_lines(invalid_test_lines, False)), 0)
